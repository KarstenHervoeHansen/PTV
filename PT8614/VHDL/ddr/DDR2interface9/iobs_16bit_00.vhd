--******************************************************************************
--
--  Xilinx, Inc. 2002                 www.xilinx.com
--
--  XAPP 253 - Synthesizable DDR SDRAM Controller
--
--*******************************************************************************
--
--  File name :       iobs.vhd
--
--  Description :     
--                    In this module all the inputs and outputs are declared 
--                    using the components in the IOB's
--
--  Date - revision : 05/01/2002
--
--  Author :          Lakshmi Gopalakrishnan (Modified by Sailaja)
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
use work.parameter_16bit_00.all;
--
-- pragma translate_off
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--
entity   iobs_16bit_00  is
port(
     clk               : in std_logic;
     clk90             : in std_logic;  
  clk180  : in std_logic;
  clk270  : in std_logic;
     ddr_rasb_cntrl    : in std_logic;
     ddr_casb_cntrl    : in std_logic;
     ddr_web_cntrl     : in std_logic;
     ddr_cke_cntrl     : in std_logic;
     ddr_csb_cntrl     : in std_logic;
     ddr_ODT_cntrl     : in std_logic;
     ddr_address_cntrl : in std_logic_vector((row_address_p -1) downto 0);
     ddr_ba_cntrl      : in std_logic_vector((bank_address_p -1) downto 0);
     rst_dqs_div_int   : in std_logic;
     dqs_reset         : in std_logic;
     dqs_enable        : in std_logic;
     ddr_dqs           : inout std_logic_vector(1 downto 0);
     ddr_dqs_n         : inout std_logic_vector(1 downto 0);
     ddr_dq            : inout std_logic_vector(15 downto 0);
     write_data_falling: in std_logic_vector(15 downto 0);
     write_data_rising : in std_logic_vector(15 downto 0);
     write_en_val      : in std_logic;
     reset270_r        : in std_logic;
     data_mask_f       : in std_logic_vector(1 downto 0);
     data_mask_r       : in std_logic_vector(1 downto 0);
     ddr2_clk0         : out std_logic;
     ddr2_clk0b        : out std_logic;
     ddr_rasb          : out std_logic;
     ddr_casb          : out std_logic;
     ddr_web           : out std_logic;
     ddr_ba            : out std_logic_vector((bank_address_p -1) downto 0);
     ddr_address       : out std_logic_vector((row_address_p -1) downto 0);
     ddr_cke           : out std_logic;
     ddr_csb           : out std_logic;
     ddr_ODT0          : out std_logic;
     rst_dqs_div       : out std_logic; 
     rst_dqs_div_in    : in std_logic;
     rst_dqs_div_out   : out std_logic;
     dqs_int_delay_in0 : out std_logic;
     dqs_int_delay_in1 : out std_logic;
     dq                : out std_logic_vector(15 downto 0);
     ddr_dm            : out std_logic_vector(1 downto 0)              
);
end   iobs_16bit_00;  


architecture   arc_iobs_16bit_00 of   iobs_16bit_00    is

component	infrastructure_iobs_16bit 
port(
     clk0              : in std_logic;
     clk90             : in std_logic;
  clk180  : in std_logic;
  clk270  : in std_logic;
     ddr2_clk0         : out std_logic;
     ddr2_clk0b        : out std_logic
     );
end component;


component controller_iobs_16bit_00 
port(
     clk0             : in std_logic;
  clk180  : in std_logic;
     ddr_rasb_cntrl   : in std_logic;
     ddr_casb_cntrl   : in std_logic;
     ddr_web_cntrl    : in std_logic;
     ddr_cke_cntrl    : in std_logic;
     ddr_csb_cntrl    : in std_logic;
     ddr_ODT_cntrl    : in std_logic;
     ddr_address_cntrl: in std_logic_vector((row_address_p -1) downto 0);
     ddr_ba_cntrl     : in std_logic_vector((bank_address_p -1) downto 0);
     rst_dqs_div_int  : in std_logic;
     ddr_rasb         : out std_logic;
     ddr_casb         : out std_logic;
     ddr_web          : out std_logic;
     ddr_ba           : out std_logic_vector((bank_address_p -1) downto 0);
     ddr_address      : out std_logic_vector((row_address_p -1) downto 0);
     ddr_cke          : out std_logic;
     ddr_csb          : out std_logic;
     ddr_ODT0         : out std_logic;
     rst_dqs_div      : out std_logic;
     rst_dqs_div_in   : in std_logic;
     rst_dqs_div_out  : out std_logic
    );
end component;

component	data_path_iobs_16bit 
port(
     clk               : in std_logic;
  clk180  : in std_logic;
  clk270  : in std_logic;
     dqs_reset         : in std_logic;
     dqs_enable        : in std_logic;
     ddr_dqs           : inout std_logic_vector(1 downto 0);
     ddr_dqs_n         : inout std_logic_vector(1 downto 0);
     ddr_dq            : inout std_logic_vector(15 downto 0);
     write_data_falling: in std_logic_vector(15 downto 0);
     write_data_rising : in std_logic_vector(15 downto 0);
     write_en_val      : in std_logic;
     clk90             : in std_logic;
     reset270_r        : in std_logic;
     data_mask_f       : in std_logic_vector(1 downto 0);
     data_mask_r       : in std_logic_vector(1 downto 0);
     dqs_int_delay_in0 : out std_logic;
     dqs_int_delay_in1 : out std_logic;
     dq                : out std_logic_vector(15 downto 0);
     ddr_dm            : out std_logic_vector(1 downto 0)  
);
end component;           

begin

infrastructure_iobs0	:	infrastructure_iobs_16bit	port	map	( 
                                                     clk0             => clk,
                                                     clk90            => clk90,
          clk180=> clk180,
          clk270=> clk270,
                                                     ddr2_clk0        => ddr2_clk0,
                                                     ddr2_clk0b       => ddr2_clk0b
                                                    );

controller_iobs0 : controller_iobs_16bit_00 port map (
                                             clk0              =>  clk,
          clk180=> clk180,
                                             ddr_rasb_cntrl    =>  ddr_rasb_cntrl,
                                             ddr_casb_cntrl    =>  ddr_casb_cntrl,
                                             ddr_web_cntrl     =>  ddr_web_cntrl, 
                                             ddr_cke_cntrl     =>  ddr_cke_cntrl,
                                             ddr_csb_cntrl     =>  ddr_csb_cntrl,
                                             ddr_ODT_cntrl     =>  ddr_ODT_cntrl,
                                             ddr_address_cntrl =>  ddr_address_cntrl((row_address_p -1) downto 0),
                                             ddr_ba_cntrl      =>  ddr_ba_cntrl((bank_address_p -1) downto 0),
                                             rst_dqs_div_int   =>  rst_dqs_div_int,
                                             ddr_rasb          =>  ddr_rasb,
                                             ddr_casb          =>  ddr_casb,
                                             ddr_web           =>  ddr_web,
                                             ddr_ba            =>  ddr_ba((bank_address_p -1) downto 0),
                                             ddr_address       =>  ddr_address((row_address_p -1) downto 0),
                                             ddr_cke           =>  ddr_cke,
                                             ddr_csb           =>  ddr_csb, 
                                             ddr_ODT0          =>  ddr_ODT0, 
                                             rst_dqs_div       =>  rst_dqs_div,
                                             rst_dqs_div_in	 => rst_dqs_div_in,
					     rst_dqs_div_out	 => rst_dqs_div_out
                                            );

data_path_iobs0	:	data_path_iobs_16bit	port	map	( 
                                         clk                =>   clk,
          clk180=> clk180,
          clk270=> clk270,
                                         dqs_reset          =>   dqs_reset,
                                         dqs_enable         =>   dqs_enable,
                                         ddr_dqs            =>   ddr_dqs(1 downto 0),
                                         ddr_dqs_n          =>   ddr_dqs_n(1 downto 0),
                                         ddr_dq             =>   ddr_dq(15 downto 0),
                                         write_data_falling =>   write_data_falling(15 downto 0),
                                         write_data_rising  =>   write_data_rising(15 downto 0),
                                         write_en_val       =>   write_en_val,
                                         clk90              =>   clk90,
                                         reset270_r         =>   reset270_r,
                                         data_mask_f        =>   data_mask_f(1 downto 0),
                                         data_mask_r        =>   data_mask_r(1 downto 0),
                                         dqs_int_delay_in0  =>   dqs_int_delay_in0,
                                         dqs_int_delay_in1  =>   dqs_int_delay_in1,
                                         dq                 =>   dq(15 downto 0),
                                         ddr_dm             =>   ddr_dm(1 downto 0)
                                        );

   

end   arc_iobs_16bit_00;  
         
 




   
