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

signal mReset              : std_logic := '0';
signal Clk                 : std_logic := '1';

signal count_reset         : std_logic := '0';
signal sync_count          : std_logic_vector(23 downto 0);
signal sync                : std_logic;
signal synctype1           : std_logic;
signal synctype2           : std_logic;

constant sync_period 	   : integer := 59;
constant spl_div_const     : integer := 2;
constant lpf_const    	   : integer := 3;
constant delay_const   	   : integer := 0;

component analog_levels
Port (
   mreset_i      : in std_logic;    -- active low
   frame_reset_i : in std_logic;    -- active high
   clk_i         : in std_logic;
   sync_i        : in std_logic;
   sync_t_i      : in std_logic;
   tsg_out_o     : out std_logic_vector(3 downto 0)
   );
end component;


begin


analog_leveling : analog_levels
 	port map (
		mreset_i     	=> mReset,
      frame_reset_i  => sync,
      clk_i          => clk,
      sync_i         => synctype1,
      sync_t_i       => synctype2
   );    


Clk <= not Clk after 0.5 ns;
mReset <= '1' after 5 ns;
count_reset <=	'0';

synctype1 <= '0';
synctype2 <= '1';


Generate_sync: process (mReset,Clk) 
begin
  if mReset = '0' then
    sync_count <= (others => '0');
    sync <= '0';
  elsif Clk 'event and Clk  = '1' then
    if count_reset = '1' or sync_count = conv_std_logic_vector(0,24) then
    	 sync_count <= conv_std_logic_vector(sync_period-1,24);
	 sync <= '1'; 
    else
	 sync_count <= sync_count - 1;
	 sync <= '0';
    end if;
  end if;
end process;  


end behavioral;