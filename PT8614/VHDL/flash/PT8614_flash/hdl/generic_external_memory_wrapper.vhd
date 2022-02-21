-------------------------------------------------------------------------------
-- generic_external_memory_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library plb_emc_v2_00_a;
use plb_emc_v2_00_a.All;

entity generic_external_memory_wrapper is
  port (
    PLB_Clk : in std_logic;
    PLB_Rst : in std_logic;
    PLB_abort : in std_logic;
    PLB_ABus : in std_logic_vector(0 to 31);
    PLB_BE : in std_logic_vector(0 to 7);
    PLB_busLock : in std_logic;
    PLB_compress : in std_logic;
    PLB_guarded : in std_logic;
    PLB_lockErr : in std_logic;
    PLB_masterID : in std_logic_vector(0 to 0);
    PLB_MSize : in std_logic_vector(0 to 1);
    PLB_ordered : in std_logic;
    PLB_PAValid : in std_logic;
    PLB_RNW : in std_logic;
    PLB_size : in std_logic_vector(0 to 3);
    PLB_type : in std_logic_vector(0 to 2);
    Sl_addrAck : out std_logic;
    Sl_MBusy : out std_logic_vector(0 to 1);
    Sl_MErr : out std_logic_vector(0 to 1);
    Sl_rearbitrate : out std_logic;
    Sl_SSize : out std_logic_vector(0 to 1);
    Sl_wait : out std_logic;
    PLB_rdPrim : in std_logic;
    PLB_SAValid : in std_logic;
    PLB_wrPrim : in std_logic;
    PLB_wrBurst : in std_logic;
    PLB_wrDBus : in std_logic_vector(0 to 63);
    Sl_wrBTerm : out std_logic;
    Sl_wrComp : out std_logic;
    Sl_wrDAck : out std_logic;
    PLB_rdBurst : in std_logic;
    Sl_rdBTerm : out std_logic;
    Sl_rdComp : out std_logic;
    Sl_rdDAck : out std_logic;
    Sl_rdDBus : out std_logic_vector(0 to 63);
    Sl_rdWdAddr : out std_logic_vector(0 to 3);
    PLB_pendReq : in std_logic;
    PLB_pendPri : in std_logic_vector(0 to 1);
    PLB_reqPri : in std_logic_vector(0 to 1);
    Mem_A : out std_logic_vector(0 to 31);
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
    Mem_RNW : out std_logic;
    Mem_DQ_I : in std_logic_vector(0 to 15);
    Mem_DQ_O : out std_logic_vector(0 to 15);
    Mem_DQ_T : out std_logic_vector(0 to 15)
  );
  attribute x_core_info : STRING;
  attribute x_core_info of generic_external_memory_wrapper: entity is "plb_emc_v2_00_a";

end generic_external_memory_wrapper;

architecture STRUCTURE of generic_external_memory_wrapper is

  component plb_emc is
    generic (
      C_NUM_BANKS_MEM : INTEGER;
      C_INCLUDE_NEGEDGE_IOREGS : INTEGER;
      C_NUM_MASTERS : INTEGER;
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
      C_PLB_DWIDTH : INTEGER;
      C_PLB_AWIDTH : INTEGER;
      C_PLB_CLK_PERIOD_PS : INTEGER;
      C_INCLUDE_BURST_CACHELN_SUPPORT : INTEGER;
      C_PLB_MID_WIDTH : INTEGER
    );
    port (
      PLB_Clk : in std_logic;
      PLB_Rst : in std_logic;
      PLB_abort : in std_logic;
      PLB_ABus : in std_logic_vector(0 to (C_PLB_AWIDTH-1));
      PLB_BE : in std_logic_vector(0 to ((C_PLB_DWIDTH/8)-1));
      PLB_busLock : in std_logic;
      PLB_compress : in std_logic;
      PLB_guarded : in std_logic;
      PLB_lockErr : in std_logic;
      PLB_masterID : in std_logic_vector(0 to (C_PLB_MID_WIDTH-1));
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_ordered : in std_logic;
      PLB_PAValid : in std_logic;
      PLB_RNW : in std_logic;
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      Sl_addrAck : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to (C_NUM_MASTERS-1));
      Sl_MErr : out std_logic_vector(0 to (C_NUM_MASTERS-1));
      Sl_rearbitrate : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      PLB_rdPrim : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_wrBurst : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to (C_PLB_DWIDTH-1));
      Sl_wrBTerm : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_wrDAck : out std_logic;
      PLB_rdBurst : in std_logic;
      Sl_rdBTerm : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_rdDAck : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to (C_PLB_DWIDTH-1));
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      PLB_pendReq : in std_logic;
      PLB_pendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      Mem_A : out std_logic_vector(0 to (C_PLB_AWIDTH-1));
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
      Mem_RNW : out std_logic;
      Mem_DQ_I : in std_logic_vector(0 to (C_MAX_MEM_WIDTH-1));
      Mem_DQ_O : out std_logic_vector(0 to (C_MAX_MEM_WIDTH-1));
      Mem_DQ_T : out std_logic_vector(0 to (C_MAX_MEM_WIDTH-1))
    );
  end component;

begin

  generic_external_memory : plb_emc
    generic map (
      C_NUM_BANKS_MEM => 1,
      C_INCLUDE_NEGEDGE_IOREGS => 0,
      C_NUM_MASTERS => 2,
      C_MEM0_BASEADDR => X"00000000",
      C_MEM0_HIGHADDR => X"01ffffff",
      C_MEM1_BASEADDR => X"ffffffff",
      C_MEM1_HIGHADDR => X"00000000",
      C_MEM2_BASEADDR => X"ffffffff",
      C_MEM2_HIGHADDR => X"00000000",
      C_MEM3_BASEADDR => X"ffffffff",
      C_MEM3_HIGHADDR => X"00000000",
      C_MEM0_WIDTH => 16,
      C_MEM1_WIDTH => 64,
      C_MEM2_WIDTH => 64,
      C_MEM3_WIDTH => 64,
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
      C_THZCE_PS_MEM_1 => 8000,
      C_THZOE_PS_MEM_1 => 8000,
      C_TWC_PS_MEM_1 => 15000,
      C_TWP_PS_MEM_1 => 12000,
      C_TLZWE_PS_MEM_1 => 5000,
      C_SYNCH_MEM_2 => 0,
      C_SYNCH_PIPEDELAY_2 => 2,
      C_TCEDV_PS_MEM_2 => 15000,
      C_TAVDV_PS_MEM_2 => 15000,
      C_THZCE_PS_MEM_2 => 8000,
      C_THZOE_PS_MEM_2 => 8000,
      C_TWC_PS_MEM_2 => 15000,
      C_TWP_PS_MEM_2 => 12000,
      C_TLZWE_PS_MEM_2 => 5000,
      C_SYNCH_MEM_3 => 0,
      C_SYNCH_PIPEDELAY_3 => 2,
      C_TCEDV_PS_MEM_3 => 15000,
      C_TAVDV_PS_MEM_3 => 15000,
      C_THZCE_PS_MEM_3 => 8000,
      C_THZOE_PS_MEM_3 => 8000,
      C_TWC_PS_MEM_3 => 15000,
      C_TWP_PS_MEM_3 => 12000,
      C_TLZWE_PS_MEM_3 => 5000,
      C_PLB_DWIDTH => 64,
      C_PLB_AWIDTH => 32,
      C_PLB_CLK_PERIOD_PS => 13468,
      C_INCLUDE_BURST_CACHELN_SUPPORT => 1,
      C_PLB_MID_WIDTH => 1
    )
    port map (
      PLB_Clk => PLB_Clk,
      PLB_Rst => PLB_Rst,
      PLB_abort => PLB_abort,
      PLB_ABus => PLB_ABus,
      PLB_BE => PLB_BE,
      PLB_busLock => PLB_busLock,
      PLB_compress => PLB_compress,
      PLB_guarded => PLB_guarded,
      PLB_lockErr => PLB_lockErr,
      PLB_masterID => PLB_masterID,
      PLB_MSize => PLB_MSize,
      PLB_ordered => PLB_ordered,
      PLB_PAValid => PLB_PAValid,
      PLB_RNW => PLB_RNW,
      PLB_size => PLB_size,
      PLB_type => PLB_type,
      Sl_addrAck => Sl_addrAck,
      Sl_MBusy => Sl_MBusy,
      Sl_MErr => Sl_MErr,
      Sl_rearbitrate => Sl_rearbitrate,
      Sl_SSize => Sl_SSize,
      Sl_wait => Sl_wait,
      PLB_rdPrim => PLB_rdPrim,
      PLB_SAValid => PLB_SAValid,
      PLB_wrPrim => PLB_wrPrim,
      PLB_wrBurst => PLB_wrBurst,
      PLB_wrDBus => PLB_wrDBus,
      Sl_wrBTerm => Sl_wrBTerm,
      Sl_wrComp => Sl_wrComp,
      Sl_wrDAck => Sl_wrDAck,
      PLB_rdBurst => PLB_rdBurst,
      Sl_rdBTerm => Sl_rdBTerm,
      Sl_rdComp => Sl_rdComp,
      Sl_rdDAck => Sl_rdDAck,
      Sl_rdDBus => Sl_rdDBus,
      Sl_rdWdAddr => Sl_rdWdAddr,
      PLB_pendReq => PLB_pendReq,
      PLB_pendPri => PLB_pendPri,
      PLB_reqPri => PLB_reqPri,
      Mem_A => Mem_A,
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
      Mem_RNW => Mem_RNW,
      Mem_DQ_I => Mem_DQ_I,
      Mem_DQ_O => Mem_DQ_O,
      Mem_DQ_T => Mem_DQ_T
    );

end architecture STRUCTURE;

