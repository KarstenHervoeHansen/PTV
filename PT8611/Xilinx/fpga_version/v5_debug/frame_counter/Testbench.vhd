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

signal count_reset         : std_logic :='0';
signal sync_count          : std_logic_vector(23 downto 0);
signal sync                : std_logic;

signal spl_div             : std_logic_vector(9 downto 0);  
signal div10_11            : std_logic;                     
signal lpf                 : std_logic_vector(10 downto 0);       
signal phasedelay          : std_logic_vector(25 downto 0); 
signal frame_sync	         : std_logic;

constant sync_period 	   : integer := 120;
constant spl_div_const     : integer := 4;
constant lpf_const    	   : integer := 3;

component frame_counter
   port (
	   mreset_i      	      : in std_logic;                     -- Master reset
	   frame_start_i        : in std_logic;                     -- tick at frame start
	   clk_i         	      : in std_logic;                     -- sample clock
	   spl_div_i            : in std_logic_vector(9 downto 0);  -- total samples pr line divided by 10 or 11
      div10_11_i           : in std_logic;                     -- divider f148 med 10 hvis lav, 11 hvis høj
	   start_line_pulse_o   : out std_logic;
	   mid_line_pulse_o     : out std_logic;
	   line_number_o        : out std_logic_vector(10 downto 0)
	);
end component;


begin

spl_div <= conv_std_logic_vector(spl_div_const,10);
lpf <= conv_std_logic_vector(lpf_const,11);
div10_11 <= '0';


frame_counting : frame_counter
 	port map (
		mreset_i    	=> mReset,
      clk_i          => clk,
      frame_start_i  => sync,
      spl_div_i      => spl_div,
      div10_11_i     => div10_11
   );    


Clk <= not Clk after 0.5 ns;
mReset <= '1' after 5 ns;
count_reset <=	'0', '1' after 10 ns, '0' after 11 ns ;


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