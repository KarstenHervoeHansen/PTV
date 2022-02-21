--******************************************************************************
--
--  Xilinx, Inc. 2002                 www.xilinx.com
--
--
--*******************************************************************************
--
--  File name :       ddr2_dm.vhd
--
--  Description :     This module instantiates DDR IOB output flip-flops, and an 
--                    output buffer for the data mask bits.   
--                   
--  Date - revision : 07/31/2003
--
--  Author :          Maria George
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
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
--use work.parameter_16bit.all;

-- pragma translate_off
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--

entity   ddr2_dm_16bit  is
port (
      ddr_dm       : out std_logic_vector(1 downto 0);   --Data mask output
      mask_falling : in std_logic_vector(1 downto 0);    --Mask output on falling edge
      mask_rising  : in std_logic_vector(1 downto 0);    --Mask output on rising edge
  clk270 : in std_logic;
      clk90        : in std_logic   --Clock 90
      ); 
end   ddr2_dm_16bit;  

architecture   arc_ddr2_dm_16bit of   ddr2_dm_16bit    is

attribute syn_keep : boolean;

component FDDRRSE 
port( Q  : out std_logic;
      C0 : in std_logic;
      C1 : in std_logic;
      CE : in std_logic;
      D0 : in std_logic;
      D1 : in std_logic;
      R  : in std_logic;
      S  : in std_logic);
end component;

component OBUF
port (
       I : in std_logic;
       O : out std_logic);
end component;





--***********************************************************************\
--     Internal signal declaration
--***********************************************************************/
  
signal mask_o    : std_logic_vector(1 downto 0);  -- Mask output intermediate signal
signal gnd       : std_logic;
signal vcc       : std_logic;


begin

gnd      <= '0';
vcc      <= '1';

-- Data Mask Output during a write command

DDR_DM0_OUT : FDDRRSE port map (
                               Q  => mask_o(0), 
                               C0 => clk270, 
                               C1 => clk90, 
                               CE => vcc,
                               D0 => mask_rising(0), 
                               D1 => mask_falling(0), 
                               R  => gnd, 
                               S  => gnd
                              );

DDR_DM1_OUT : FDDRRSE port map (
                               Q  => mask_o(1), 
                               C0 => clk270, 
                               C1 => clk90, 
                               CE => vcc,
                               D0 => mask_rising(1), 
                               D1 => mask_falling(1), 
                               R  => gnd, 
                               S  => gnd
                              );
                              
                                                                      
DM0_OBUF : OBUF port map (
                         I => mask_o(0),
                         O => ddr_dm(0)
                        );

DM1_OBUF : OBUF port map (
                         I => mask_o(1),
                         O => ddr_dm(1)
                        );
                        
                        
end   arc_ddr2_dm_16bit;  

