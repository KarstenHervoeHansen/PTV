-- ****************************************************************************
-- Copyright (c) 2005 DK-Technologies. All Rights Reserved
-- Proprietary Information - Internal Data
-- ----------------------------------------------------------------------------
-- File Name: TestBench.vhd
-- 26 May 2005
--
-- Author(s): Thomas Holm Hansen
-- 
-- Description: TestBench for f4m_genlock_regen.vhd
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

signal Clk            : std_logic  :='1';
signal mReset         : std_logic  :='0';
signal f4m            : std_logic  :='0';
signal f2m            : std_logic;

signal count_reset    : std_logic 	:='0';
signal f4m_count      : std_logic_vector(23 downto 0);
constant f4m_period   : integer := 20;				 -- 5940000


component f4m_genlock_regen  
  port (
    mreset_i          : in std_logic;
    clk_i             : in std_logic;
    f4m_i             : in std_logic;
    f2m_o             : out std_logic
    );
end component;


-- component HD_frame  
--   port ( 
--     clk :       	in std_logic;                       -- sample clock
--     mute :      	in std_logic;                       -- active low
--     -- video registers :
--     hdformat :  	in std_logic_vector(3 downto 0);    -- SMPTE 240M, 274M or 296M
--     -- active video from video_sm :
--     act_y :     	in std_logic_vector(9 downto 0);
--     act_cb :    	in std_logic_vector(9 downto 0);
--     act_cr :    	in std_logic_vector(9 downto 0);
--     -- outputs for reading new format data :
--     endof_frame :   out std_logic;
--     -- outputs for video_sm :
--     sav :           out std_logic;                      -- start of active video
--     fvh :           out std_logic_vector(2 downto 0);   -- Field, Vert-blank and Horz-blank bits
--     line_clk :      inout std_logic;                    -- total line clock
--     -- parallel video out :
--     luma :      	out std_logic_vector(9 downto 0);
--     chroma :    	out std_logic_vector(9 downto 0)
--   );
-- end component;


begin

Genlock_regen_1 : f4m_genlock_regen
  port map (
    mreset_i        => mReset,
    clk_i           => Clk,
    f4m_i           => f4m,
    f2m_o           => f2m
  );    


-- HD_frame_1 : HD_frame
--   port ( 
--     clk :       	in std_logic;                       -- sample clock
--     mute :      	in std_logic;                       -- active low
--     -- video registers :
--     hdformat :  	in std_logic_vector(3 downto 0);    -- SMPTE 240M, 274M or 296M
--     -- active video from video_sm :
--     act_y :     	in std_logic_vector(9 downto 0);
--     act_cb :    	in std_logic_vector(9 downto 0);
--     act_cr :    	in std_logic_vector(9 downto 0);
--     -- outputs for reading new format data :
--     endof_frame :   out std_logic;
--     -- outputs for video_sm :
--     sav :           out std_logic;                      -- start of active video
--     fvh :           out std_logic_vector(2 downto 0);   -- Field, Vert-blank and Horz-blank bits
--     line_clk :      inout std_logic;                    -- total line clock
--     -- parallel video out :
--     luma :      	out std_logic_vector(9 downto 0);
--     chroma :    	out std_logic_vector(9 downto 0)
--   );
  

Clk <= not Clk after 0.5 ns;
mReset <= '1' after 5 ns;
count_reset <=	'0', '1' after 104 ns, '0' after 105 ns;


Generate_f4m: process (mReset,Clk) 
begin
  if mReset = '0' then
    f4m_count <= (others => '0');
    f4m <= '0';
  elsif Clk 'event and Clk  = '1' then
    if count_reset = '1' or f4m_count = conv_std_logic_vector(0,24) then
    	 f4m_count <= conv_std_logic_vector(f4m_period-1,24);
	 f4m <= '1'; 
    else
	 f4m_count <= f4m_count - 1;
	 f4m <= '0';
    end if;
  end if;
end process;  

end behavioral;