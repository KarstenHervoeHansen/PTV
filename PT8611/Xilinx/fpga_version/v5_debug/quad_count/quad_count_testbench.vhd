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

entity quad_count_testbench is
end quad_count_testbench;

architecture behavioral of quad_count_testbench is

signal mReset              : std_logic := '0';
signal Clk                 : std_logic := '1';

signal sync_reset          : std_logic :='0';
signal sync                : std_logic;

constant count1        : integer := 2; 
constant count2        : integer := 3; 
constant count3        : integer := 4; 
constant count4        : integer := 5; 
constant bits1         : integer := 3;
constant bits2         : integer := 3;
constant bits3         : integer := 3;
constant bits4         : integer := 3;



component period_quad_count
   generic(
      count_val1  : integer;            -- must be >= 2
      count_val2  : integer;            -- must be >= 2
      count_val3  : integer;            -- must be >= 2
      count_val4  : integer;            -- must be >= 2
      bits1       : integer;
      bits2       : integer;
      bits3       : integer;
      bits4       : integer
   );
   Port(
      mreset_i    : in std_logic;
      clk_i       : in std_logic;
      sync_reset_i: in std_logic;
      sync_o      : out std_logic
   );
end component;                


begin


sync24_generation : period_quad_count 
generic map (
   count_val1 => count1,
   count_val2 => count2,
   count_val3 => count3,
   count_val4 => count4,
   bits1      => bits1, 
   bits2      => bits2, 
   bits3      => bits3, 
   bits4      => bits4 
)
port map (
   mreset_i     => mreset,
   clk_i        => clk,
   sync_reset_i => sync_reset,
   sync_o       => sync
);


Clk <= not Clk after 0.5 ns;
mReset <= '1' after 5 ns;
sync_reset <=	'0', '1' after 257 ns, '0' after 258 ns;

end behavioral;