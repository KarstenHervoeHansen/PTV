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

entity FSD_Testbench is
end FSD_Testbench;

architecture behavioral of FSD_Testbench is

signal mReset              : std_logic := '0';
signal Clk                 : std_logic := '1';

signal sync_reset          : std_logic :='0';
signal sync_count          : std_logic_vector(23 downto 0);
signal sync                : std_logic;

signal spl_div             : std_logic_vector(9 downto 0);  
signal div10_11            : std_logic;                     
signal lpf                 : std_logic_vector(10 downto 0);       
signal phasedelay          : std_logic_vector(23 downto 0); 
signal frame_sync	         : std_logic;

constant sync_period 	   : integer := 60;
constant spl_div_const     : integer := 2;
constant lpf_const    	   : integer := 3;
constant delay_const   	   : integer := 0;

constant sync24_count1     : integer := 4;  -- used to generate 24Hz sync
constant sync24_count2     : integer := 5;  -- 2250*2750 = 6187500
constant sync24_bits1      : integer := 3;  -- size of counter 1
constant sync24_bits2      : integer := 3;  -- size of counter 2


component period_dual_count
   generic(
      count_val1 : integer;
      count_val2 : integer;
      bits1      : integer;
      bits2      : integer
   );
   Port(
      mreset_i    : in std_logic;
      clk_i       : in std_logic;
      sync_reset_i: in std_logic;
      sync_o      : out std_logic
   );
end component;                


begin

spl_div <= conv_std_logic_vector(spl_div_const,10);
lpf <= conv_std_logic_vector(lpf_const,11);
phasedelay <= conv_std_logic_vector(delay_const,24);
div10_11 <= '0';


sync24_generation : period_dual_count 
generic map (
   count_val1 => sync24_count1,
   count_val2 => sync24_count2,
   bits1      => sync24_bits1, 
   bits2      => sync24_bits2 
)
port map (
   mreset_i     => mreset,
   clk_i        => clk,
   sync_reset_i => sync_reset,
   sync_o       => sync
);


Clk <= not Clk after 0.5 ns;
mReset <= '1' after 5 ns;
sync_reset <=	'0', '1' after 57 ns, '0' after 58 ns;

end behavioral;