-------------------------------------------------------------------------------
-- plb_bram_if_cntlr_1_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library plb_bram_if_cntlr_v1_00_b;
use plb_bram_if_cntlr_v1_00_b.All;

entity plb_bram_if_cntlr_1_wrapper is
  port (
    plb_clk : in std_logic;
    plb_rst : in std_logic;
    plb_abort : in std_logic;
    plb_abus : in std_logic_vector(0 to 31);
    plb_be : in std_logic_vector(0 to 7);
    plb_buslock : in std_logic;
    plb_compress : in std_logic;
    plb_guarded : in std_logic;
    plb_lockerr : in std_logic;
    plb_masterid : in std_logic_vector(0 to 0);
    plb_msize : in std_logic_vector(0 to 1);
    plb_ordered : in std_logic;
    plb_pavalid : in std_logic;
    plb_rnw : in std_logic;
    plb_size : in std_logic_vector(0 to 3);
    plb_type : in std_logic_vector(0 to 2);
    sl_addrack : out std_logic;
    sl_mbusy : out std_logic_vector(0 to 1);
    sl_merr : out std_logic_vector(0 to 1);
    sl_rearbitrate : out std_logic;
    sl_ssize : out std_logic_vector(0 to 1);
    sl_wait : out std_logic;
    plb_rdprim : in std_logic;
    plb_savalid : in std_logic;
    plb_wrprim : in std_logic;
    plb_wrburst : in std_logic;
    plb_wrdbus : in std_logic_vector(0 to 63);
    sl_wrbterm : out std_logic;
    sl_wrcomp : out std_logic;
    sl_wrdack : out std_logic;
    plb_rdburst : in std_logic;
    sl_rdbterm : out std_logic;
    sl_rdcomp : out std_logic;
    sl_rddack : out std_logic;
    sl_rddbus : out std_logic_vector(0 to 63);
    sl_rdwdaddr : out std_logic_vector(0 to 3);
    plb_pendreq : in std_logic;
    plb_pendpri : in std_logic_vector(0 to 1);
    plb_reqpri : in std_logic_vector(0 to 1);
    bram_rst : out std_logic;
    bram_clk : out std_logic;
    bram_en : out std_logic;
    bram_wen : out std_logic_vector(0 to 7);
    bram_addr : out std_logic_vector(0 to 31);
    bram_din : in std_logic_vector(0 to 63);
    bram_dout : out std_logic_vector(0 to 63)
  );
  attribute x_core_info : STRING;
  attribute x_core_info of plb_bram_if_cntlr_1_wrapper: entity is "plb_bram_if_cntlr_v1_00_b";

end plb_bram_if_cntlr_1_wrapper;

architecture STRUCTURE of plb_bram_if_cntlr_1_wrapper is

  component plb_bram_if_cntlr is
    generic (
      c_num_masters : integer;
      c_baseaddr : std_logic_vector;
      c_highaddr : std_logic_vector;
      c_include_burst_cacheln_support : integer;
      c_plb_dwidth : integer;
      c_plb_awidth : integer;
      c_plb_clk_period_ps : integer;
      c_plb_mid_width : integer
    );
    port (
      plb_clk : in std_logic;
      plb_rst : in std_logic;
      plb_abort : in std_logic;
      plb_abus : in std_logic_vector(0 to (c_plb_awidth-1));
      plb_be : in std_logic_vector(0 to ((c_plb_dwidth/8)-1));
      plb_buslock : in std_logic;
      plb_compress : in std_logic;
      plb_guarded : in std_logic;
      plb_lockerr : in std_logic;
      plb_masterid : in std_logic_vector(0 to (c_plb_mid_width-1));
      plb_msize : in std_logic_vector(0 to 1);
      plb_ordered : in std_logic;
      plb_pavalid : in std_logic;
      plb_rnw : in std_logic;
      plb_size : in std_logic_vector(0 to 3);
      plb_type : in std_logic_vector(0 to 2);
      sl_addrack : out std_logic;
      sl_mbusy : out std_logic_vector(0 to (c_num_masters-1));
      sl_merr : out std_logic_vector(0 to (c_num_masters-1));
      sl_rearbitrate : out std_logic;
      sl_ssize : out std_logic_vector(0 to 1);
      sl_wait : out std_logic;
      plb_rdprim : in std_logic;
      plb_savalid : in std_logic;
      plb_wrprim : in std_logic;
      plb_wrburst : in std_logic;
      plb_wrdbus : in std_logic_vector(0 to (c_plb_dwidth-1));
      sl_wrbterm : out std_logic;
      sl_wrcomp : out std_logic;
      sl_wrdack : out std_logic;
      plb_rdburst : in std_logic;
      sl_rdbterm : out std_logic;
      sl_rdcomp : out std_logic;
      sl_rddack : out std_logic;
      sl_rddbus : out std_logic_vector(0 to (c_plb_dwidth-1));
      sl_rdwdaddr : out std_logic_vector(0 to 3);
      plb_pendreq : in std_logic;
      plb_pendpri : in std_logic_vector(0 to 1);
      plb_reqpri : in std_logic_vector(0 to 1);
      bram_rst : out std_logic;
      bram_clk : out std_logic;
      bram_en : out std_logic;
      bram_wen : out std_logic_vector(0 to ((c_plb_dwidth/8)-1));
      bram_addr : out std_logic_vector(0 to (c_plb_awidth-1));
      bram_din : in std_logic_vector(0 to (c_plb_dwidth-1));
      bram_dout : out std_logic_vector(0 to (c_plb_dwidth-1))
    );
  end component;

begin

  plb_bram_if_cntlr_1 : plb_bram_if_cntlr
    generic map (
      c_num_masters => 2,
      c_baseaddr => X"03000000",
      c_highaddr => X"03007fff",
      c_include_burst_cacheln_support => 0,
      c_plb_dwidth => 64,
      c_plb_awidth => 32,
      c_plb_clk_period_ps => 13468,
      c_plb_mid_width => 1
    )
    port map (
      plb_clk => plb_clk,
      plb_rst => plb_rst,
      plb_abort => plb_abort,
      plb_abus => plb_abus,
      plb_be => plb_be,
      plb_buslock => plb_buslock,
      plb_compress => plb_compress,
      plb_guarded => plb_guarded,
      plb_lockerr => plb_lockerr,
      plb_masterid => plb_masterid,
      plb_msize => plb_msize,
      plb_ordered => plb_ordered,
      plb_pavalid => plb_pavalid,
      plb_rnw => plb_rnw,
      plb_size => plb_size,
      plb_type => plb_type,
      sl_addrack => sl_addrack,
      sl_mbusy => sl_mbusy,
      sl_merr => sl_merr,
      sl_rearbitrate => sl_rearbitrate,
      sl_ssize => sl_ssize,
      sl_wait => sl_wait,
      plb_rdprim => plb_rdprim,
      plb_savalid => plb_savalid,
      plb_wrprim => plb_wrprim,
      plb_wrburst => plb_wrburst,
      plb_wrdbus => plb_wrdbus,
      sl_wrbterm => sl_wrbterm,
      sl_wrcomp => sl_wrcomp,
      sl_wrdack => sl_wrdack,
      plb_rdburst => plb_rdburst,
      sl_rdbterm => sl_rdbterm,
      sl_rdcomp => sl_rdcomp,
      sl_rddack => sl_rddack,
      sl_rddbus => sl_rddbus,
      sl_rdwdaddr => sl_rdwdaddr,
      plb_pendreq => plb_pendreq,
      plb_pendpri => plb_pendpri,
      plb_reqpri => plb_reqpri,
      bram_rst => bram_rst,
      bram_clk => bram_clk,
      bram_en => bram_en,
      bram_wen => bram_wen,
      bram_addr => bram_addr,
      bram_din => bram_din,
      bram_dout => bram_dout
    );

end architecture STRUCTURE;

