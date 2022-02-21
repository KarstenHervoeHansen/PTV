-------------------------------------------------------------------------------
-- generic_external_memory_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library opb_emc_v2_00_a;
use opb_emc_v2_00_a.All;

entity generic_external_memory_wrapper is
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
  attribute x_core_info : STRING;
  attribute x_core_info of generic_external_memory_wrapper: entity is "opb_emc_v2_00_a";

end generic_external_memory_wrapper;

architecture STRUCTURE of generic_external_memory_wrapper is

  component opb_emc is
    generic (
      C_NUM_BANKS_MEM : INTEGER;
      C_INCLUDE_BURST : INTEGER;
      C_INCLUDE_NEGEDGE_IOREGS : INTEGER;
      C_FAMILY : STRING;
      C_MEM0_BASEADDR : std_logic_vector;
      C_MEM0_HIGHADDR : std_logic_vector;
      C_MEM1_BASEADDR : std_logic_vector;
      C_MEM1_HIGHADDR : std_logic_vector;
      C_MEM2_BASEADDR : std_logic_vector;
      C_MEM2_HIGHADDR : std_logic_vector;
      C_MEM3_BASEADDR : std_logic_vector;
      C_MEM3_HIGHADDR : std_logic_vector;
      C_MEM0_WIDTH : INTEGER;
      C_MEM1_WIDTH : INTEGER;
      C_MEM2_WIDTH : INTEGER;
      C_MEM3_WIDTH : INTEGER;
      C_MAX_MEM_WIDTH : INTEGER;
      C_INCLUDE_DATAWIDTH_MATCHING_0 : INTEGER;
      C_INCLUDE_DATAWIDTH_MATCHING_1 : INTEGER;
      C_INCLUDE_DATAWIDTH_MATCHING_2 : INTEGER;
      C_INCLUDE_DATAWIDTH_MATCHING_3 : INTEGER;
      C_SYNCH_MEM_0 : INTEGER;
      C_SYNCH_PIPEDELAY_0 : INTEGER;
      C_TCEDV_PS_MEM_0 : INTEGER;
      C_TAVDV_PS_MEM_0 : INTEGER;
      C_THZCE_PS_MEM_0 : INTEGER;
      C_THZOE_PS_MEM_0 : INTEGER;
      C_TWC_PS_MEM_0 : INTEGER;
      C_TWP_PS_MEM_0 : INTEGER;
      C_TLZWE_PS_MEM_0 : INTEGER;
      C_SYNCH_MEM_1 : INTEGER;
      C_SYNCH_PIPEDELAY_1 : INTEGER;
      C_TCEDV_PS_MEM_1 : INTEGER;
      C_TAVDV_PS_MEM_1 : INTEGER;
      C_THZCE_PS_MEM_1 : INTEGER;
      C_THZOE_PS_MEM_1 : INTEGER;
      C_TWC_PS_MEM_1 : INTEGER;
      C_TWP_PS_MEM_1 : INTEGER;
      C_TLZWE_PS_MEM_1 : INTEGER;
      C_SYNCH_MEM_2 : INTEGER;
      C_SYNCH_PIPEDELAY_2 : INTEGER;
      C_TCEDV_PS_MEM_2 : INTEGER;
      C_TAVDV_PS_MEM_2 : INTEGER;
      C_THZCE_PS_MEM_2 : INTEGER;
      C_THZOE_PS_MEM_2 : INTEGER;
      C_TWC_PS_MEM_2 : INTEGER;
      C_TWP_PS_MEM_2 : INTEGER;
      C_TLZWE_PS_MEM_2 : INTEGER;
      C_SYNCH_MEM_3 : INTEGER;
      C_SYNCH_PIPEDELAY_3 : INTEGER;
      C_TCEDV_PS_MEM_3 : INTEGER;
      C_TAVDV_PS_MEM_3 : INTEGER;
      C_THZCE_PS_MEM_3 : INTEGER;
      C_THZOE_PS_MEM_3 : INTEGER;
      C_TWC_PS_MEM_3 : INTEGER;
      C_TWP_PS_MEM_3 : INTEGER;
      C_TLZWE_PS_MEM_3 : INTEGER;
      C_OPB_DWIDTH : INTEGER;
      C_OPB_AWIDTH : INTEGER;
      C_OPB_CLK_PERIOD_PS : INTEGER
    );
    port (
      OPB_Clk : in std_logic;
      OPB_Rst : in std_logic;
      OPB_ABus : in std_logic_vector(0 to (C_OPB_AWIDTH-1));
      OPB_DBus : in std_logic_vector(0 to (C_OPB_DWIDTH-1));
      Sln_DBus : out std_logic_vector(0 to (C_OPB_DWIDTH-1));
      OPB_select : in std_logic;
      OPB_RNW : in std_logic;
      OPB_seqAddr : in std_logic;
      OPB_BE : in std_logic_vector(0 to ((C_OPB_DWIDTH/8)-1));
      Sln_xferAck : out std_logic;
      Sln_errAck : out std_logic;
      Sln_toutSup : out std_logic;
      Sln_retry : out std_logic;
      Mem_A : out std_logic_vector(0 to (C_OPB_AWIDTH-1));
      Mem_DQ_I : in std_logic_vector(0 to (C_MAX_MEM_WIDTH-1));
      Mem_DQ_O : out std_logic_vector(0 to (C_MAX_MEM_WIDTH-1));
      Mem_DQ_T : out std_logic_vector(0 to (C_MAX_MEM_WIDTH-1));
      Mem_CEN : out std_logic_vector(0 to (C_NUM_BANKS_MEM-1));
      Mem_OEN : out std_logic_vector(0 to (C_NUM_BANKS_MEM-1));
      Mem_WEN : out std_logic;
      Mem_QWEN : out std_logic_vector(0 to ((C_MAX_MEM_WIDTH/8)-1));
      Mem_BEN : out std_logic_vector(0 to ((C_MAX_MEM_WIDTH/8)-1));
      Mem_RPN : out std_logic;
      Mem_CE : out std_logic_vector(0 to (C_NUM_BANKS_MEM-1));
      Mem_ADV_LDN : out std_logic;
      Mem_LBON : out std_logic;
      Mem_CKEN : out std_logic;
      Mem_RNW : out std_logic
    );
  end component;

begin

  generic_external_memory : opb_emc
    generic map (
      C_NUM_BANKS_MEM => 1,
      C_INCLUDE_BURST => 0,
      C_INCLUDE_NEGEDGE_IOREGS => 0,
      C_FAMILY => "virtex2p",
      C_MEM0_BASEADDR => X"24000000",
      C_MEM0_HIGHADDR => X"25ffffff",
      C_MEM1_BASEADDR => X"ffffffff",
      C_MEM1_HIGHADDR => X"00000000",
      C_MEM2_BASEADDR => X"ffffffff",
      C_MEM2_HIGHADDR => X"00000000",
      C_MEM3_BASEADDR => X"ffffffff",
      C_MEM3_HIGHADDR => X"00000000",
      C_MEM0_WIDTH => 16,
      C_MEM1_WIDTH => 32,
      C_MEM2_WIDTH => 32,
      C_MEM3_WIDTH => 32,
      C_MAX_MEM_WIDTH => 16,
      C_INCLUDE_DATAWIDTH_MATCHING_0 => 0,
      C_INCLUDE_DATAWIDTH_MATCHING_1 => 1,
      C_INCLUDE_DATAWIDTH_MATCHING_2 => 1,
      C_INCLUDE_DATAWIDTH_MATCHING_3 => 1,
      C_SYNCH_MEM_0 => 0,
      C_SYNCH_PIPEDELAY_0 => 2,
      C_TCEDV_PS_MEM_0 => 85000,
      C_TAVDV_PS_MEM_0 => 85000,
      C_THZCE_PS_MEM_0 => 10000,
      C_THZOE_PS_MEM_0 => 7000,
      C_TWC_PS_MEM_0 => 55000,
      C_TWP_PS_MEM_0 => 55000,
      C_TLZWE_PS_MEM_0 => 35000,
      C_SYNCH_MEM_1 => 0,
      C_SYNCH_PIPEDELAY_1 => 2,
      C_TCEDV_PS_MEM_1 => 15000,
      C_TAVDV_PS_MEM_1 => 15000,
      C_THZCE_PS_MEM_1 => 7000,
      C_THZOE_PS_MEM_1 => 7000,
      C_TWC_PS_MEM_1 => 15000,
      C_TWP_PS_MEM_1 => 12000,
      C_TLZWE_PS_MEM_1 => 0,
      C_SYNCH_MEM_2 => 0,
      C_SYNCH_PIPEDELAY_2 => 2,
      C_TCEDV_PS_MEM_2 => 15000,
      C_TAVDV_PS_MEM_2 => 15000,
      C_THZCE_PS_MEM_2 => 7000,
      C_THZOE_PS_MEM_2 => 7000,
      C_TWC_PS_MEM_2 => 15000,
      C_TWP_PS_MEM_2 => 12000,
      C_TLZWE_PS_MEM_2 => 0,
      C_SYNCH_MEM_3 => 0,
      C_SYNCH_PIPEDELAY_3 => 2,
      C_TCEDV_PS_MEM_3 => 15000,
      C_TAVDV_PS_MEM_3 => 15000,
      C_THZCE_PS_MEM_3 => 7000,
      C_THZOE_PS_MEM_3 => 7000,
      C_TWC_PS_MEM_3 => 15000,
      C_TWP_PS_MEM_3 => 12000,
      C_TLZWE_PS_MEM_3 => 0,
      C_OPB_DWIDTH => 32,
      C_OPB_AWIDTH => 32,
      C_OPB_CLK_PERIOD_PS => 13468
    )
    port map (
      OPB_Clk => OPB_Clk,
      OPB_Rst => OPB_Rst,
      OPB_ABus => OPB_ABus,
      OPB_DBus => OPB_DBus,
      Sln_DBus => Sln_DBus,
      OPB_select => OPB_select,
      OPB_RNW => OPB_RNW,
      OPB_seqAddr => OPB_seqAddr,
      OPB_BE => OPB_BE,
      Sln_xferAck => Sln_xferAck,
      Sln_errAck => Sln_errAck,
      Sln_toutSup => Sln_toutSup,
      Sln_retry => Sln_retry,
      Mem_A => Mem_A,
      Mem_DQ_I => Mem_DQ_I,
      Mem_DQ_O => Mem_DQ_O,
      Mem_DQ_T => Mem_DQ_T,
      Mem_CEN => Mem_CEN,
      Mem_OEN => Mem_OEN,
      Mem_WEN => Mem_WEN,
      Mem_QWEN => Mem_QWEN,
      Mem_BEN => Mem_BEN,
      Mem_RPN => Mem_RPN,
      Mem_CE => Mem_CE,
      Mem_ADV_LDN => Mem_ADV_LDN,
      Mem_LBON => Mem_LBON,
      Mem_CKEN => Mem_CKEN,
      Mem_RNW => Mem_RNW
    );

end architecture STRUCTURE;

