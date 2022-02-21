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

signal count_reset         : std_logic :='0';
signal sync_count          : std_logic_vector(23 downto 0);
signal sync                : std_logic;

signal spl_div             : std_logic_vector(9 downto 0);  
signal div10_11            : std_logic;                     
signal lpf                 : std_logic_vector(10 downto 0);       
signal phasedelay          : std_logic_vector(23 downto 0); 
signal frame_sync	         : std_logic;

constant sync_period 	   : integer := 1000;
constant spl_div_const     : integer := 10;
constant lpf_const    	   : integer := 3;
constant delay_const   	   : integer := 2;

component frame_sync_delay
   port ( 
	   mreset_i      	 : in std_logic;                     -- Master reset
	   clk_i         	 : in std_logic;                     -- sample clock
	   sync_i          : in std_logic;                     
	   spl_div_i       : in std_logic_vector(9 downto 0);  -- total samples pr line divided by 10 or 11
      div10_11_i      : in std_logic;                     -- divider f148 med 10 hvis lav, 11 hvis høj
	   lpf_i           : in std_logic_vector(10 downto 0); -- total lines per frame
	   samplerate_x2_i : in std_logic;
      phasedelay_i    : in std_logic_vector(23 downto 0); -- delay from sync reference
	                                                       -- Input parameters must always be valid
	   frame_sync_o	 : out std_logic
   );
end component;


begin

spl_div <= conv_std_logic_vector(spl_div_const,10);
lpf <= conv_std_logic_vector(lpf_const,11);
phasedelay <= conv_std_logic_vector(delay_const,24);
div10_11 <= '0';


frame_sync_delaying : frame_sync_delay
 	port map (
		mreset_i    	   => mReset,
      clk_i             => clk,
      sync_i            => sync,
      spl_div_i         => spl_div,
      div10_11_i        => div10_11,
      lpf_i             => lpf,
  	   samplerate_x2_i   => '0',
                        
      phasedelay_i      => phasedelay,
      frame_sync_o      => frame_sync
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