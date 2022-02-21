-------------------------------------------------------------------------------
-- opb_bram_if_cntlr_1_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library opb_bram_if_cntlr_v1_00_a;
use opb_bram_if_cntlr_v1_00_a.All;

entity opb_bram_if_cntlr_1_wrapper is
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
  attribute x_core_info : STRING;
  attribute x_core_info of opb_bram_if_cntlr_1_wrapper: entity is "opb_bram_if_cntlr_v1_00_a";

end opb_bram_if_cntlr_1_wrapper;

architecture STRUCTURE of opb_bram_if_cntlr_1_wrapper is

  component opb_bram_if_cntlr is
    generic (
      c_baseaddr : std_logic_vector;
      c_highaddr : std_logic_vector;
      c_include_burst_support : integer;
      c_opb_dwidth : integer;
      c_opb_awidth : integer;
      c_opb_clk_period_ps : integer
    );
    port (
      opb_clk : in std_logic;
      opb_rst : in std_logic;
      opb_abus : in std_logic_vector(0 to (c_opb_awidth-1));
      opb_dbus : in std_logic_vector(0 to (c_opb_dwidth-1));
      sln_dbus : out std_logic_vector(0 to (c_opb_dwidth-1));
      opb_select : in std_logic;
      opb_rnw : in std_logic;
      opb_seqaddr : in std_logic;
      opb_be : in std_logic_vector(0 to ((c_opb_dwidth/8)-1));
      sln_xferack : out std_logic;
      sln_errack : out std_logic;
      sln_toutsup : out std_logic;
      sln_retry : out std_logic;
      bram_rst : out std_logic;
      bram_clk : out std_logic;
      bram_en : out std_logic;
      bram_wen : out std_logic_vector(0 to ((c_opb_dwidth/8)-1));
      bram_addr : out std_logic_vector(0 to (c_opb_awidth-1));
      bram_din : in std_logic_vector(0 to (c_opb_dwidth-1));
      bram_dout : out std_logic_vector(0 to (c_opb_dwidth-1))
    );
  end component;

begin

  opb_bram_if_cntlr_1 : opb_bram_if_cntlr
    generic map (
      c_baseaddr => X"26000000",
      c_highaddr => X"26003fff",
      c_include_burst_support => 0,
      c_opb_dwidth => 32,
      c_opb_awidth => 32,
      c_opb_clk_period_ps => 13468
    )
    port map (
      opb_clk => opb_clk,
      opb_rst => opb_rst,
      opb_abus => opb_abus,
      opb_dbus => opb_dbus,
      sln_dbus => sln_dbus,
      opb_select => opb_select,
      opb_rnw => opb_rnw,
      opb_seqaddr => opb_seqaddr,
      opb_be => opb_be,
      sln_xferack => sln_xferack,
      sln_errack => sln_errack,
      sln_toutsup => sln_toutsup,
      sln_retry => sln_retry,
      bram_rst => bram_rst,
      bram_clk => bram_clk,
      bram_en => bram_en,
      bram_wen => bram_wen,
      bram_addr => bram_addr,
      bram_din => bram_din,
      bram_dout => bram_dout
    );

end architecture STRUCTURE;

