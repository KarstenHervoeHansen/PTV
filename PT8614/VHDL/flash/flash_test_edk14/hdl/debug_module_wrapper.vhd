-------------------------------------------------------------------------------
-- debug_module_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library opb_mdm_v2_00_a;
use opb_mdm_v2_00_a.All;

entity debug_module_wrapper is
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
  attribute x_core_info : STRING;
  attribute x_core_info of debug_module_wrapper: entity is "opb_mdm_v2_00_a";

end debug_module_wrapper;

architecture STRUCTURE of debug_module_wrapper is

  component opb_mdm is
    generic (
      C_BASEADDR : STD_LOGIC_VECTOR;
      C_HIGHADDR : STD_LOGIC_VECTOR;
      C_OPB_DWIDTH : INTEGER;
      C_OPB_AWIDTH : INTEGER;
      C_FAMILY : STRING;
      C_MB_DBG_PORTS : INTEGER;
      C_USE_UART : INTEGER;
      C_UART_WIDTH : INTEGER;
      C_WRITE_FSL_PORTS : INTEGER
    );
    port (
      OPB_Clk : in std_logic;
      OPB_Rst : in std_logic;
      Interrupt : out std_logic;
      Debug_SYS_Rst : out std_logic;
      Debug_Rst : out std_logic;
      Ext_BRK : out std_logic;
      Ext_NM_BRK : out std_logic;
      OPB_ABus : in std_logic_vector(0 to C_OPB_AWIDTH-1);
      OPB_BE : in std_logic_vector(0 to C_OPB_DWIDTH/8-1);
      OPB_RNW : in std_logic;
      OPB_select : in std_logic;
      OPB_seqAddr : in std_logic;
      OPB_DBus : in std_logic_vector(0 to C_OPB_DWIDTH-1);
      MDM_DBus : out std_logic_vector(0 to C_OPB_DWIDTH-1);
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

begin

  debug_module : opb_mdm
    generic map (
      C_BASEADDR => X"41400000",
      C_HIGHADDR => X"4140ffff",
      C_OPB_DWIDTH => 32,
      C_OPB_AWIDTH => 32,
      C_FAMILY => "virtex2p",
      C_MB_DBG_PORTS => 1,
      C_USE_UART => 1,
      C_UART_WIDTH => 8,
      C_WRITE_FSL_PORTS => 0
    )
    port map (
      OPB_Clk => OPB_Clk,
      OPB_Rst => OPB_Rst,
      Interrupt => Interrupt,
      Debug_SYS_Rst => Debug_SYS_Rst,
      Debug_Rst => Debug_Rst,
      Ext_BRK => Ext_BRK,
      Ext_NM_BRK => Ext_NM_BRK,
      OPB_ABus => OPB_ABus,
      OPB_BE => OPB_BE,
      OPB_RNW => OPB_RNW,
      OPB_select => OPB_select,
      OPB_seqAddr => OPB_seqAddr,
      OPB_DBus => OPB_DBus,
      MDM_DBus => MDM_DBus,
      MDM_errAck => MDM_errAck,
      MDM_retry => MDM_retry,
      MDM_toutSup => MDM_toutSup,
      MDM_xferAck => MDM_xferAck,
      Dbg_Clk_0 => Dbg_Clk_0,
      Dbg_TDI_0 => Dbg_TDI_0,
      Dbg_TDO_0 => Dbg_TDO_0,
      Dbg_Reg_En_0 => Dbg_Reg_En_0,
      Dbg_Capture_0 => Dbg_Capture_0,
      Dbg_Update_0 => Dbg_Update_0,
      Dbg_Clk_1 => Dbg_Clk_1,
      Dbg_TDI_1 => Dbg_TDI_1,
      Dbg_TDO_1 => Dbg_TDO_1,
      Dbg_Reg_En_1 => Dbg_Reg_En_1,
      Dbg_Capture_1 => Dbg_Capture_1,
      Dbg_Update_1 => Dbg_Update_1,
      Dbg_Clk_2 => Dbg_Clk_2,
      Dbg_TDI_2 => Dbg_TDI_2,
      Dbg_TDO_2 => Dbg_TDO_2,
      Dbg_Reg_En_2 => Dbg_Reg_En_2,
      Dbg_Capture_2 => Dbg_Capture_2,
      Dbg_Update_2 => Dbg_Update_2,
      Dbg_Clk_3 => Dbg_Clk_3,
      Dbg_TDI_3 => Dbg_TDI_3,
      Dbg_TDO_3 => Dbg_TDO_3,
      Dbg_Reg_En_3 => Dbg_Reg_En_3,
      Dbg_Capture_3 => Dbg_Capture_3,
      Dbg_Update_3 => Dbg_Update_3,
      Dbg_Clk_4 => Dbg_Clk_4,
      Dbg_TDI_4 => Dbg_TDI_4,
      Dbg_TDO_4 => Dbg_TDO_4,
      Dbg_Reg_En_4 => Dbg_Reg_En_4,
      Dbg_Capture_4 => Dbg_Capture_4,
      Dbg_Update_4 => Dbg_Update_4,
      Dbg_Clk_5 => Dbg_Clk_5,
      Dbg_TDI_5 => Dbg_TDI_5,
      Dbg_TDO_5 => Dbg_TDO_5,
      Dbg_Reg_En_5 => Dbg_Reg_En_5,
      Dbg_Capture_5 => Dbg_Capture_5,
      Dbg_Update_5 => Dbg_Update_5,
      Dbg_Clk_6 => Dbg_Clk_6,
      Dbg_TDI_6 => Dbg_TDI_6,
      Dbg_TDO_6 => Dbg_TDO_6,
      Dbg_Reg_En_6 => Dbg_Reg_En_6,
      Dbg_Capture_6 => Dbg_Capture_6,
      Dbg_Update_6 => Dbg_Update_6,
      Dbg_Clk_7 => Dbg_Clk_7,
      Dbg_TDI_7 => Dbg_TDI_7,
      Dbg_TDO_7 => Dbg_TDO_7,
      Dbg_Reg_En_7 => Dbg_Reg_En_7,
      Dbg_Capture_7 => Dbg_Capture_7,
      Dbg_Update_7 => Dbg_Update_7,
      bscan_tdi => bscan_tdi,
      bscan_reset => bscan_reset,
      bscan_shift => bscan_shift,
      bscan_update => bscan_update,
      bscan_capture => bscan_capture,
      bscan_sel1 => bscan_sel1,
      bscan_drck1 => bscan_drck1,
      bscan_tdo1 => bscan_tdo1,
      FSL0_S_CLK => FSL0_S_CLK,
      FSL0_S_READ => FSL0_S_READ,
      FSL0_S_DATA => FSL0_S_DATA,
      FSL0_S_CONTROL => FSL0_S_CONTROL,
      FSL0_S_EXISTS => FSL0_S_EXISTS,
      FSL0_M_CLK => FSL0_M_CLK,
      FSL0_M_WRITE => FSL0_M_WRITE,
      FSL0_M_DATA => FSL0_M_DATA,
      FSL0_M_CONTROL => FSL0_M_CONTROL,
      FSL0_M_FULL => FSL0_M_FULL
    );

end architecture STRUCTURE;

