--******************************************************************************
--
--  Xilinx, Inc. 2002                 www.xilinx.com
--
--  XAPP 253 - Synthesizable DDR SDRAM Controller
--
--*******************************************************************************
--
--  File name :       infrastructure.vhd
--
--  Description :     
--                    Main fucntions of this module
--                       - generation of FPGA clocks.
--                       - generation of reset signals
--                       - implements calibration mechanism
--
--  Date - revision : 05/01/2002
--
--  Author :          Lakshmi Gopalakrishnan ( Modified by Sailja) 
--
--  Contact : e-mail  hotline@xilinx.com
--            phone   + 1 800 255 7778 
--
--  Disclaimer: LIMITED WARRANTY AND DISCLAMER. These designs are 
--              provided to you "as is". Xilinx and its licensors make and you 
--              receive no warranties or conditions, express, implied, 
--              statutory or otherwise, and Xilinx specifically disclaims any 
--              implied warranties of merchantability, non-infringement, or 
--              fitness for a particular purpose. Xilinx does not warrant that 
--              the functions contained in these designs will meet your 
--              requirements, or that the operation of these designs will be 
--              uninterrupted or error free, or that defects in the Designs 
--              will be corrected. Furthermore, Xilinx does not warrant or 
--              make any representations regarding use or the results of the 
--              use of the designs in terms of correctness, accuracy, 
--              reliability, or otherwise. 
--
--              LIMITATION OF LIABILITY. In no event will Xilinx or its 
--              licensors be liable for any loss of data, lost profits, cost 
--              or procurement of substitute goods or services, or for any 
--              special, incidental, consequential, or indirect damages 
--              arising from the use or operation of the designs or 
--              accompanying documentation, however caused and on any theory 
--              of liability. This limitation will apply even if Xilinx 
--              has been advised of the possibility of such damage. This 
--              limitation shall apply not-withstanding the failure of the 
--              essential purpose of any limited remedies herein. 
--
--  Copyright © 2002 Xilinx, Inc.
--  All rights reserved 
-- 
--*****************************************************************************



library IEEE;
use IEEE.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
--library synplify; 
--use synplify.attributes.all;
--
-- pragma translate_off
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--

entity infrastructure_top is
  port(
	        reset_in         : in std_logic;
		              sys_clk_ibuf     : in std_logic;
	
       delay_sel_val1_val   : out std_logic_vector(4 downto 0);
       sys_rst_val          : out std_logic;
       sys_rst90_val        : out std_logic;
        clk_int_val          : out std_logic;
        clk90_int_val        : out std_logic;
       sys_rst180_val       : out std_logic;
       sys_rst270_val       : out std_logic;
	 wait_200us           : out std_logic

  );
  
end   infrastructure_top;

architecture arc_infrastructure_top  of infrastructure_top  is

---- Component declarations -----

  component clk_dcm 
  port(
       input_clk   : in std_logic;
       rst         : in std_logic;
       clk         : out std_logic;
       clk90       : out std_logic;
       dcm_lock    : out std_logic
       );
  end component;

component cal_top                                                 
port             (                                                
	          clk         : in std_logic;                     
	          clk0        : in std_logic;                     
	          clk0dcmlock : in std_logic;                     
	          reset       : in std_logic;                     
	          okToSelTap  : in std_logic;
	          tapForDqs   : out std_logic_vector(4 downto 0)
                 );                                               
end component;

---- Signal declarations used on the diagram ----

signal user_rst                : std_logic;
  signal clk_int                 : std_logic;
  signal clk90_int               : std_logic;
  signal dcm_lock                : std_logic;
signal sys_rst_o               : std_logic;
signal sys_rst_1               : std_logic := '1';
signal sys_rst                 : std_logic ;
signal sys_rst90_o             : std_logic;
signal sys_rst90_1             : std_logic := '1';
signal sys_rst90               : std_logic;
signal sys_rst180_o            : std_logic;
signal sys_rst180_1            : std_logic := '1';
signal sys_rst180              : std_logic ;
signal sys_rst270_o            : std_logic;
signal sys_rst270_1            : std_logic := '1';
signal sys_rst270              : std_logic ;
signal delay_sel_val           : std_logic_vector(4 downto 0);
signal vcc                     : std_logic;

signal clk_int_val1            : std_logic;
signal clk_int_val2            : std_logic;
signal clk90_int_val1          : std_logic;
signal clk90_int_val2          : std_logic;

signal wait_200us_i            : std_logic;
signal wait_clk90           : std_logic;
signal Counter200              : std_logic_vector(15 downto 0);
begin

  clk_int_val <= clk_int;
  clk90_int_val <= clk90_int;


sys_rst_val <= sys_rst;
sys_rst90_val <= sys_rst90;
sys_rst180_val <= sys_rst180;
sys_rst270_val <= sys_rst270;
delay_sel_val1_val <= delay_sel_val;
-----   To remove delta delays in the clock signals observed during simulation ,Following signals are used 

clk_int_val1 <= clk_int;
clk90_int_val1 <= clk90_int;
clk_int_val2 <= clk_int_val1;
clk90_int_val2 <= clk90_int_val1;

vcc       <= '1';
user_rst  <= not reset_in;  

wait_200us <= wait_200us_i;
--For 200us during power up
process(clk_int_val2)
begin
 if clk_int_val2'event and clk_int_val2 = '1' then
  if user_rst = '1' or dcm_lock = '0' then
     wait_200us_i     <= '1';
     Counter200       <= (others => '0');
  else
      if( Counter200(15) = '1' and Counter200(14) = '1' and Counter200(13) = '1' and wait_200us_i = '1') then
         wait_200us_i <='0';
      elsif (wait_200us_i = '1') then
         Counter200 <= Counter200 + 1;
      else    
         Counter200 <= Counter200;
     end if;
  end if;
 end if;
end process;


process(clk90_int_val2)
begin
 if clk90_int_val2'event and clk90_int_val2 = '1' then
  if user_rst = '1' or dcm_lock = '0' then
      wait_clk90 <= '1';
   else
      wait_clk90 <= wait_200us_i;
  end if;
 end if;
end process;

                                      

process(clk_int_val2)
begin
 if clk_int_val2'event and clk_int_val2 = '1' then
  if user_rst = '1' or dcm_lock = '0' or wait_200us_i = '1'  then
      sys_rst_o <= '1';
      sys_rst_1 <= '1';
      sys_rst   <= '1';
  else
      sys_rst_o <= '0';
      sys_rst_1 <= sys_rst_o;
      sys_rst   <= sys_rst_1;
  end if;
 end if;
end process;

process(clk90_int_val2)
begin
 if clk90_int_val2'event and clk90_int_val2 = '1' then
  if user_rst = '1' or dcm_lock = '0' or wait_clk90 = '1'  then
      sys_rst90_o <= '1';
      sys_rst90_1 <= '1';
      sys_rst90   <= '1';
  else
      sys_rst90_o <= '0';
      sys_rst90_1 <= sys_rst90_o;
      sys_rst90   <= sys_rst90_1;
  end if;
 end if;
end process;

process(clk_int_val2)
begin
 if clk_int_val2'event and clk_int_val2 = '0' then
  if user_rst = '1' or dcm_lock = '0' or wait_200us_i = '1'  then
      sys_rst180_o <= '1';
      sys_rst180_1 <= '1';
      sys_rst180   <= '1';
  else
      sys_rst180_o <= '0';
      sys_rst180_1 <= sys_rst180_o;
      sys_rst180   <= sys_rst180_1;
  end if;
 end if;
end process;

process(clk90_int_val2)
begin
 if clk90_int_val2'event and clk90_int_val2 = '0' then
  if user_rst = '1' or dcm_lock = '0' or wait_clk90 = '1'  then
      sys_rst270_o <= '1';
      sys_rst270_1 <= '1';
      sys_rst270   <= '1';
  else
      sys_rst270_o <= '0';
      sys_rst270_1 <= sys_rst270_o;
      sys_rst270   <= sys_rst270_1;
  end if;
 end if;
end process;

----  Component instantiations  ----

                                   
  clk_dcm0 : clk_dcm port map (
                               input_clk   => sys_clk_ibuf,
                               rst         => user_rst,                            
                               clk         => clk_int,
                               clk90       => clk90_int,
                               dcm_lock    => dcm_lock
                              ); 
                            
cal_top0 : cal_top port map (                                                 
                             clk         => sys_clk_ibuf,  
                             clk0        => clk_int_val2,                
                             clk0dcmlock => dcm_lock,  
                             reset       => reset_in,      
                             okToSelTap  => vcc,
                             tapForDqs   => delay_sel_val
                             );       
                                          

end arc_infrastructure_top ;


