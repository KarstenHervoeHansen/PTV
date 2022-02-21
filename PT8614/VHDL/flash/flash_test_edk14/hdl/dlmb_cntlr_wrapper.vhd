-------------------------------------------------------------------------------
-- dlmb_cntlr_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library lmb_bram_if_cntlr_v1_00_b;
use lmb_bram_if_cntlr_v1_00_b.All;

entity dlmb_cntlr_wrapper is
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
  attribute x_core_info : STRING;
  attribute x_core_info of dlmb_cntlr_wrapper: entity is "lmb_bram_if_cntlr_v1_00_b";

end dlmb_cntlr_wrapper;

architecture STRUCTURE of dlmb_cntlr_wrapper is

  component lmb_bram_if_cntlr is
    generic (
      C_BASEADDR : std_logic_vector(0 to 31);
      C_HIGHADDR : std_logic_vector(0 to 31);
      C_MASK : std_logic_vector(0 to 31);
      C_LMB_AWIDTH : integer;
      C_LMB_DWIDTH : integer
    );
    port (
      LMB_Clk : in std_logic;
      LMB_Rst : in std_logic;
      LMB_ABus : in std_logic_vector(0 to C_LMB_AWIDTH-1);
      LMB_WriteDBus : in std_logic_vector(0 to C_LMB_DWIDTH-1);
      LMB_AddrStrobe : in std_logic;
      LMB_ReadStrobe : in std_logic;
      LMB_WriteStrobe : in std_logic;
      LMB_BE : in std_logic_vector(0 to C_LMB_DWIDTH/8-1);
      Sl_DBus : out std_logic_vector(0 to C_LMB_DWIDTH-1);
      Sl_Ready : out std_logic;
      BRAM_Rst_A : out std_logic;
      BRAM_Clk_A : out std_logic;
      BRAM_EN_A : out std_logic;
      BRAM_WEN_A : out std_logic_vector(0 to (C_LMB_DWIDTH/8)-1);
      BRAM_Addr_A : out std_logic_vector(0 to C_LMB_AWIDTH-1);
      BRAM_Din_A : in std_logic_vector(0 to C_LMB_DWIDTH-1);
      BRAM_Dout_A : out std_logic_vector(0 to C_LMB_DWIDTH-1)
    );
  end component;

begin

  dlmb_cntlr : lmb_bram_if_cntlr
    generic map (
      C_BASEADDR => X"00000000",
      C_HIGHADDR => X"00007fff",
      C_MASK => X"04400000",
      C_LMB_AWIDTH => 32,
      C_LMB_DWIDTH => 32
    )
    port map (
      LMB_Clk => LMB_Clk,
      LMB_Rst => LMB_Rst,
      LMB_ABus => LMB_ABus,
      LMB_WriteDBus => LMB_WriteDBus,
      LMB_AddrStrobe => LMB_AddrStrobe,
      LMB_ReadStrobe => LMB_ReadStrobe,
      LMB_WriteStrobe => LMB_WriteStrobe,
      LMB_BE => LMB_BE,
      Sl_DBus => Sl_DBus,
      Sl_Ready => Sl_Ready,
      BRAM_Rst_A => BRAM_Rst_A,
      BRAM_Clk_A => BRAM_Clk_A,
      BRAM_EN_A => BRAM_EN_A,
      BRAM_WEN_A => BRAM_WEN_A,
      BRAM_Addr_A => BRAM_Addr_A,
      BRAM_Din_A => BRAM_Din_A,
      BRAM_Dout_A => BRAM_Dout_A
    );

end architecture STRUCTURE;

