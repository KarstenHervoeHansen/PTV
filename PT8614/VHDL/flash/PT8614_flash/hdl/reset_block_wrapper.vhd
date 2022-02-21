-------------------------------------------------------------------------------
-- reset_block_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library proc_sys_reset_v1_00_a;
use proc_sys_reset_v1_00_a.All;

entity reset_block_wrapper is
  port (
    Slowest_sync_clk : in std_logic;
    Ext_Reset_In : in std_logic;
    Aux_Reset_In : in std_logic;
    Core_Reset_Req : in std_logic;
    Chip_Reset_Req : in std_logic;
    System_Reset_Req : in std_logic;
    Dcm_locked : in std_logic;
    Rstc405resetcore : out std_logic;
    Rstc405resetchip : out std_logic;
    Rstc405resetsys : out std_logic;
    Bus_Struct_Reset : out std_logic_vector(0 to 0);
    Peripheral_Reset : out std_logic_vector(0 to 0)
  );
  attribute x_core_info : STRING;
  attribute x_core_info of reset_block_wrapper: entity is "proc_sys_reset_v1_00_a";

end reset_block_wrapper;

architecture STRUCTURE of reset_block_wrapper is

  component proc_sys_reset is
    generic (
      C_EXT_RST_WIDTH : integer;
      C_AUX_RST_WIDTH : integer;
      C_EXT_RESET_HIGH : std_logic;
      C_AUX_RESET_HIGH : std_logic;
      C_NUM_BUS_RST : integer;
      C_NUM_PERP_RST : integer
    );
    port (
      Slowest_sync_clk : in std_logic;
      Ext_Reset_In : in std_logic;
      Aux_Reset_In : in std_logic;
      Core_Reset_Req : in std_logic;
      Chip_Reset_Req : in std_logic;
      System_Reset_Req : in std_logic;
      Dcm_locked : in std_logic;
      Rstc405resetcore : out std_logic;
      Rstc405resetchip : out std_logic;
      Rstc405resetsys : out std_logic;
      Bus_Struct_Reset : out std_logic_vector(0 to C_NUM_BUS_RST-1);
      Peripheral_Reset : out std_logic_vector(0 to C_NUM_PERP_RST-1)
    );
  end component;

begin

  reset_block : proc_sys_reset
    generic map (
      C_EXT_RST_WIDTH => 4,
      C_AUX_RST_WIDTH => 4,
      C_EXT_RESET_HIGH => '0',
      C_AUX_RESET_HIGH => '1',
      C_NUM_BUS_RST => 1,
      C_NUM_PERP_RST => 1
    )
    port map (
      Slowest_sync_clk => Slowest_sync_clk,
      Ext_Reset_In => Ext_Reset_In,
      Aux_Reset_In => Aux_Reset_In,
      Core_Reset_Req => Core_Reset_Req,
      Chip_Reset_Req => Chip_Reset_Req,
      System_Reset_Req => System_Reset_Req,
      Dcm_locked => Dcm_locked,
      Rstc405resetcore => Rstc405resetcore,
      Rstc405resetchip => Rstc405resetchip,
      Rstc405resetsys => Rstc405resetsys,
      Bus_Struct_Reset => Bus_Struct_Reset,
      Peripheral_Reset => Peripheral_Reset
    );

end architecture STRUCTURE;

