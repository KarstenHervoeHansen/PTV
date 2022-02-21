-- ****************************************************************************
-- Copyright (c) 2005 DK-Technologies. All Rights Reserved
-- Proprietary Information - Internal Data
-- ----------------------------------------------------------------------------
-- File Name: Mainframe.vhd
-- 26 May 2005
--
-- Author(s): Thomas Holm Hansen
-- 
-- Description: TestBench for trilevel sync generator
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

entity Mainframe is
end Mainframe;

architecture behavioral of Mainframe is

signal mReset              : std_logic := '0';
signal Clk                 : std_logic := '1';

signal count_reset         : std_logic 	:='0';
signal sync_count          : std_logic_vector(23 downto 0);
signal f1485               : std_logic;
signal f1484               : std_logic;
signal sync                : std_logic;
signal f4m                 : std_logic;
signal f8g                 : std_logic;

signal led1                : std_logic;
signal led2                : std_logic;
signal led3                : std_logic;

signal ext30               : std_logic;
signal ext31               : std_logic;
signal ext32               : std_logic;
signal ext33               : std_logic;

constant sync_period 	   : integer := 20;		     	      -- 9900000 used for NTSC
--constant f8g_sync_period 	: integer := 23760000;		     	-- 23760000 used for PAL


component Tri_Level_Module
   Port (
      --------- til/fra Mainframe -------------------------------------------------------------------
      mreset_i :    in std_logic;      -- Master Reset fra mainboard
      f1484_i  :    in std_logic;      -- 148.35 MHz clock
      f1485_i  :    in std_logic;      -- 148.5 MHz clock
      f4m_i    :    in std_logic;      -- Vertikal sync reference - 625 lines SD format
      f8g_i    :    in std_logic;      -- Vertikal sync reference - 525 lines SD format

      --------- testpoints -------------------------------------------------------
      led1_o   :    out std_logic;     -- forbundet til en noconnect på xc3s50
      led2_o   :    out std_logic;     -- forbundet til en noconnect på xc3s50
      led3_o   :    out std_logic;

      ext30_o  :    out std_logic;     -- j5 pin 1
      ext31_o  :    out std_logic;     -- j5 pin 2
      ext32_o  :    out std_logic;     -- j5 pin 3
      ext33_o  :    out std_logic      -- j5 pin 4
   );
end component;


begin

trilevel_module_1 : Tri_Level_Module
 	port map (
		mreset_i    	=> mReset,
		
      f1484_i        => f1484,
      f1485_i        => f1485,
      f4m_i          => f4m,
      f8g_i          => f8g,
      --------- testpoints ------------------------------------------------
      led1_o         => led1,
      led2_o         => led2,
      led3_o         => led3,

      ext30_o        => ext30,
      ext31_o        => ext31,
      ext32_o        => ext32,
      ext33_o        => ext33
   );    


Clk <= not Clk after 0.5 ns;
mReset <= '1' after 5 ns;
count_reset <=	'0', '1' after 10 ns, '0' after 11 ns, '1' after 64 ns, '0' after 65 ns, '1' after 103 ns, '0' after 104 ns,
               '1' after 186 ns, '0' after 187 ns;


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


f1485 <= Clk;
f1484 <= Clk;

f8g <= sync;
f4m <= sync;

end behavioral;