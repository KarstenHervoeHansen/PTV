-- ****************************************************************************
-- Copyright (c) 2005 DK-Technologies. All Rights Reserved
-- Proprietary Information - Internal Data
-- ----------------------------------------------------------------------------
-- File Name: FSD_Testbench.vhd
-- 29 June 2005
--
-- Author(s): Thomas Holm Hansen
--
-- Description: TestBench for frame_sync_delay.vhd
-------------------------------------------------------------------------------

LIBRARY ieee;
USE     ieee.std_logic_1164.all;
USE     ieee.std_logic_arith.all;
USE     ieee.std_logic_signed.all;

LIBRARY std;
USE   std.standard.ALL;
USE   std.textio.ALL;


-- Library Synopsys;
-- Use Synopsys.hdl_xmr_pkg.all;
library modelsim_lib;
use modelsim_lib.util.all;


LIBRARY work;
-- USE     work.sim_pkg.all;
-- USE     work.test.all;

entity Testbench is
end Testbench;

architecture behavioral of Testbench is


signal mReset                 : std_logic := '0';
signal Clk                    : std_logic := '1';
signal sync                   : std_logic;
signal sync_reclk             : std_logic;
signal DAC                    : std_logic_vector(3 downto 0);
signal sync_60                : std_logic;


constant sync60_count1           : integer := 1375;        -- used to generate 60Hz sync
constant sync60_count2           : integer := 1800;        -- 1375*1800
constant sync60_bits1            : integer := 12;          -- size of counter 1         
constant sync60_bits2            : integer := 12;          -- size of counter 2         
                                                                                        

component period_dual_count
   generic(
      count_val1  : integer;
      count_val2  : integer;
      bits1       : integer;
      bits2       : integer
   );
   Port(
      mreset_i    : in std_logic;
      clk_i       : in std_logic;
      sync_reset_i: in std_logic;
      sync_o      : out std_logic
   );
end component;


component Tri_level_timer
Port (
   mreset_i          : in std_logic;                     -- Master reset
   frame_start_i     : in std_logic;                     -- start generator puls - aktiv høj
   clk_i             : in std_logic;                     -- multiplexed 148 MHz clock
   interlaced_i      : in std_logic;                     -- ( 1=interlaced, 0=progressiv )
   div10_11_i        : in std_logic;                     -- divider clk_i med 10 hvis lav, 11 hvis høj
   sync_mode_lsb_i   : in std_logic;
   spl_div_i         : in std_logic_vector(9 downto 0);  -- total samples pr. line divideret med 10 (prog) eller 11 (interlaced)
   lines_pr_frame_i  : in std_logic_vector(10 downto 0);
   framerate_x2_i    : in std_logic;
   tsg_lvl_i         : in std_logic;
   mute_i            : in std_logic;
   tsg_ok_o          : out std_logic;
   DAC_o             : out std_logic_vector(3 downto 0);
   timing_change_o   : out std_logic
   );
end component;


begin


sync30_generation : period_dual_count
generic map (
   count_val1 => sync60_count1,
   count_val2 => sync60_count2,
   bits1      => sync60_bits1,
   bits2      => sync60_bits2
)
port map (
   mreset_i     => mReset,
   clk_i        => Clk,
   sync_reset_i => '0',
   sync_o       => sync_60
);


tri_level_timing : tri_level_timer
 	port map (
		mreset_i          => mReset,
      frame_start_i     => sync_reclk,
      clk_i             => Clk,
      interlaced_i      => '0',
      div10_11_i        => '0',
      sync_mode_lsb_i   => '0',
      spl_div_i         => "0101001001",      -- 3300/10-1
      lines_pr_frame_i  => "01011101110",    -- 750
      framerate_x2_i    => '0',
      tsg_lvl_i         => '1',
      mute_i            => '0',
      DAC_o             => DAC
   );


Clk <= not Clk after 5 ps;
mReset <= '1' after 50 ps;
sync <= '0', '1' after 130 ps, '0' after 140 ps,
        '1' after 49500130 ps, '0' after 49500140 ps;


sync_reclocking : process(mReset, Clk)
begin
   if mReset = '0' then
      sync_reclk <= '0';
   elsif Clk'event and Clk = '1' then
      sync_reclk <= sync;
   end if;
end process;


end behavioral;