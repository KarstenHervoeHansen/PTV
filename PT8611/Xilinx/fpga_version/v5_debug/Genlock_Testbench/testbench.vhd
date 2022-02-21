-- ****************************************************************************
-- Copyright (c) 2005 DK-Technologies. All Rights Reserved
-- Proprietary Information - Internal Data
-- ----------------------------------------------------------------------------
-- File Name: TestBench.vhd
-- 26 May 2005
--
-- Author(s): Thomas Holm Hansen
-- 
-- Description: TestBench for sync_genlock_regen.vhd
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

entity testbench is
end testbench;

architecture behavioral of testbench is

signal Clk               : std_logic  :='1';
signal mReset            : std_logic  :='0';
signal f4m               : std_logic  :='0';
signal f4m_clean         : std_logic;
signal f4m_genlock_ok    : std_logic;
signal count_reset       : std_logic 	:='0';
signal f4m_count         : std_logic_vector(23 downto 0);

constant f4m_sync_period : integer := 20;		     	   -- 9900000 used for NTSC
constant count_val1      : integer := 4;
constant count_val2      : integer := 5;
constant bits1           : integer := 3;
constant bits2           : integer := 3;


component sync_genlock_regen
	generic (
   	count_val1         : integer;                             -- the period of sync is count_val1*count_val2
      count_val2         : integer;
      bits1              : integer;                             -- number of bits in counters
      bits2              : integer
	);  
  	port (
   	mreset_i           : in std_logic;
   	clk_i              : in std_logic;
   	sync_i             : in std_logic;
   	sync_clean_o       : out std_logic;
		genlock_ok_o       : out std_logic		
   );
end component;


begin

f4m_Genlock_regen : sync_genlock_regen
	generic map (
   	count_val1     => count_val1, 
      count_val2     => count_val2,
   	bits1          => bits1,     
      bits2          => bits2     
	)
 	port map (
		mreset_i    	=> mReset,
		clk_i       	=> Clk,
		sync_i      	=> f4m,
		sync_clean_o	=> f4m_clean,
		genlock_ok_o	=> f4m_genlock_ok
  );    


Clk <= not Clk after 0.5 ns;
mReset <= '1' after 5 ns;
count_reset <=	'0', '1' after 64 ns, '0' after 65 ns, '1' after 103 ns, '0' after 104 ns,
               '1' after 146 ns, '0' after 147 ns, '1' after 279 ns, '0' after 280 ns;


Generate_f4m: process (mReset,Clk) 
begin
  if mReset = '0' then
    f4m_count <= (others => '0');
    f4m <= '0';
  elsif Clk 'event and Clk  = '1' then
    if count_reset = '1' or f4m_count = conv_std_logic_vector(0,24) then
    	 f4m_count <= conv_std_logic_vector(f4m_sync_period-1,24);
	 f4m <= '1'; 
    else
	 f4m_count <= f4m_count - 1;
	 f4m <= '0';
    end if;
  end if;
end process;  

end behavioral;