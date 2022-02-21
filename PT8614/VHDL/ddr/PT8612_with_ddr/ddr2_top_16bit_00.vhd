--******************************************************************************
--
--  Xilinx, Inc. 2002                 www.xilinx.com
--
--  XAPP 253 - Synthesizable DDR SDRAM Controller
--
--*******************************************************************************
--
--  File name :       ddr2_top
--
--  Description :     
--                    Main DDR SDRAM controller block. This includes the following
--                    features:
--                    - The main controller state machine that controlls the 
--                    initialization process upon power up, as well as the 
--                    read, write, and refresh commands. 
--                    - handles the data path during READ and WRITEs.
--                    - Generates control signals for other modules, including the
--                      data strobe(DQS) signal
--
--  Date - revision : 05/01/2002
--
--  Author :          Lakshmi Gopalakrishnan ( Modified by Sailaja)
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
use work.parameter_16bit_00.all;
--library synplify; 
--use synplify.attributes.all;
--
-- pragma translate_off
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--
entity   ddr2_top_16bit_00  is
port(
dip1                  : in std_logic;   
auto_ref_req            : out std_logic; 
dip3                  : in std_logic;   
wait_200us       : in std_logic; 
  clk180 : in std_logic;
  clk270 : in std_logic;
rst_dqs_div_in        : in std_logic;
rst_dqs_div_out       : out std_logic;
reset_in              : in std_logic;   
user_input_data       : in std_logic_vector(31 downto 0);
user_data_mask        : in std_logic_vector((mask_width-1) downto 0);
user_output_data      : out std_logic_vector(31 downto 0):=(OTHERS => 'Z');
user_data_valid       : out std_logic;
user_input_address    : in std_logic_vector(((row_address_p + column_address_p)-1) downto 0);
user_bank_address     : in std_logic_vector((bank_address_p-1) downto 0);
user_config_register1 : in std_logic_vector(14 downto 0);
user_config_register2 : in std_logic_vector(12 downto 0);
user_command_register : in std_logic_vector(3 downto 0);
burst_done            : in std_logic;
user_cmd_ack          : out std_logic; 
init_val              : out std_logic;
ar_done               : out std_logic;
ddr_dqs               : inout std_logic_vector(1 downto 0);
ddr_dqs_n             : inout std_logic_vector(1 downto 0);
ddr_dq                : inout std_logic_vector(15 downto 0):= (OTHERS => 'Z');
ddr_cke               : out std_logic;
ddr_csb               : out std_logic;
ddr_rasb              : out std_logic;
ddr_casb              : out std_logic;
ddr_web               : out std_logic;
ddr_dm                : out std_logic_vector(1 downto 0);  
ddr_ba                : out std_logic_vector((bank_address_p-1) downto 0);
ddr_address           : out std_logic_vector((row_address_p-1) downto 0);
ddr_ODT0              : out std_logic;
ddr2_clk0             : out std_logic;
ddr2_clk0b            : out std_logic;
clk_int	:	in	std_logic; 
clk90_int	:	in	std_logic; 
delay_sel_val          : in std_logic_vector(4 downto 0); 
sys_rst	:	in	std_logic; 
sys_rst90	:	in	std_logic; 
sys_rst180	:	in	std_logic; 
sys_rst270	:	in	std_logic 
);
end   ddr2_top_16bit_00;  
architecture   arc_ddr2_top_16bit_00 of   ddr2_top_16bit_00    is
component controller_16bit_00 
port(
dip1             : in std_logic;
auto_ref_req            : out std_logic; 
dip3             : in std_logic;
wait_200us       : in std_logic; 
clk              : in std_logic;
  clk180 : in std_logic;
rst0             : in std_logic;
rst180           : in std_logic;
address          : in std_logic_vector(((row_address_p + column_address_p)-1) downto 0);
bank_address     : in std_logic_vector((bank_address_p-1) downto 0);
config_register1 : in std_logic_vector(14 downto 0);
config_register2 : in std_logic_vector(12 downto 0);
command_register : in std_logic_vector(3 downto 0);
burst_done       : in std_logic;
ddr_rasb_cntrl   : out std_logic;
ddr_casb_cntrl   : out std_logic;
ddr_ODT_cntrl    : out std_logic;
ddr_web_cntrl    : out std_logic;
ddr_ba_cntrl     : out std_logic_vector((bank_address_p-1) downto 0);
ddr_address_cntrl: out std_logic_vector((row_address_p-1) downto 0);
ddr_cke_cntrl    : out std_logic;
ddr_csb_cntrl    : out std_logic;
dqs_enable       : out std_logic;
dqs_reset        : out std_logic;
write_enable     : out std_logic;
rst_calib        : out std_logic;
rst_dqs_div_int  : out std_logic;
cmd_ack          : out std_logic;
init             : out std_logic;
ar_done          : out std_logic
);
end component;                                    
component	data_path_16bit_rl 
port(
user_input_data    : in std_logic_vector(31 downto 0);
user_data_mask        : in std_logic_vector((mask_width-1) downto 0);
clk                : in std_logic;
clk90              : in std_logic;
  clk180 : in std_logic;
  clk270 : in std_logic;
reset              : in std_logic;
reset90            : in std_logic;
reset180           : in std_logic;
reset270           : in std_logic;
write_enable       : in std_logic;
rst_dqs_div        : in std_logic;
delay_sel          : in std_logic_vector(4 downto 0);   
dqs_int_delay_in0  : in std_logic;
dqs_int_delay_in1  : in std_logic;
dq                 : in std_logic_vector(15 downto 0);       
u_data_val         : out std_logic;
user_output_data   : out std_logic_vector(31 downto 0);
write_en_val       : out std_logic;
reset270_r_val     : out std_logic;
data_mask_f        : out std_logic_vector(((mask_width/2)-1) downto 0);
data_mask_r        : out std_logic_vector(((mask_width/2)-1) downto 0);
write_data_falling : out std_logic_vector(15 downto 0);
write_data_rising  : out std_logic_vector(15 downto 0)
);
end component;
component infrastructure
port(
sys_rst            : in std_logic;
clk_int            : in std_logic;
rst_calib1         : in std_logic;
delay_sel_val      : in std_logic_vector(4 downto 0);
delay_sel_val1_val : out std_logic_vector(4 downto 0)
);
end component;
component	iobs_16bit_00 
port(
clk               : in std_logic;
clk90             : in std_logic;  
  clk180 : in std_logic;
  clk270 : in std_logic;
ddr_rasb_cntrl    : in std_logic;
ddr_casb_cntrl    : in std_logic;
ddr_web_cntrl     : in std_logic;
ddr_cke_cntrl     : in std_logic;
ddr_csb_cntrl     : in std_logic;
ddr_ODT_cntrl     : in std_logic;
ddr_address_cntrl : in std_logic_vector((row_address_p-1) downto 0);
ddr_ba_cntrl      : in std_logic_vector((bank_address_p-1) downto 0);
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
data_mask_f       : in std_logic_vector(((mask_width/2)-1) downto 0);
data_mask_r       : in std_logic_vector(((mask_width/2)-1) downto 0);                                                                           
ddr2_clk0         : out std_logic;
ddr2_clk0b        : out std_logic;
ddr_rasb          : out std_logic;
ddr_casb          : out std_logic;
ddr_web           : out std_logic;
ddr_ba            : out std_logic_vector((bank_address_p-1) downto 0);
ddr_address       : out std_logic_vector((row_address_p-1) downto 0);
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
end component;
-------
signal rst_calib          : std_logic;
signal delay_sel          : std_logic_vector(4 downto 0);
-----
signal write_enable       : std_logic;
signal dqs_div_rst        : std_logic;
signal dqs_enable         : std_logic;
signal dqs_reset          : std_logic;
signal dqs_int_delay_in0  : std_logic;
signal dqs_int_delay_in1  : std_logic;
signal dq                 : std_logic_vector(15 downto 0);
signal u_data_val         : std_logic;
signal write_en_val       : std_logic;
signal reset270_r         : std_logic;
signal data_mask_f        : std_logic_vector(((mask_width/2)-1) downto 0);
signal data_mask_r        : std_logic_vector(((mask_width/2)-1) downto 0);
signal write_data_falling : std_logic_vector(15 downto 0);
signal write_data_rising  : std_logic_vector(15 downto 0);
----
signal ddr_rasb_cntrl   :  std_logic;
signal ddr_casb_cntrl   :  std_logic;
signal ddr_web_cntrl    :  std_logic;
signal ddr_ba_cntrl     :  std_logic_vector((bank_address_p-1) downto 0);
signal ddr_address_cntrl:  std_logic_vector((row_address_p-1) downto 0);
signal ddr_cke_cntrl    :  std_logic;
signal ddr_csb_cntrl    :  std_logic;
signal ddr_ODT_cntrl    :  std_logic;
signal rst_dqs_div_int  :  std_logic;
begin
controller0 : controller_16bit_00 port map (
 auto_ref_req         => auto_ref_req, 
  wait_200us         => wait_200us, 
dip1               => dip1,
dip3               => dip3,
clk	=>	clk_int, 
  clk180 => clk180,
rst0               => sys_rst,
rst180             => sys_rst180,
address            => user_input_address,
bank_address       => user_bank_address,
config_register1   => user_config_register1,
config_register2   => user_config_register2,
command_register   => user_command_register,
burst_done         => burst_done,
ddr_rasb_cntrl     => ddr_rasb_cntrl,
ddr_casb_cntrl     => ddr_casb_cntrl,
ddr_web_cntrl      => ddr_web_cntrl,
ddr_ba_cntrl       => ddr_ba_cntrl,
ddr_address_cntrl  => ddr_address_cntrl,
ddr_cke_cntrl      => ddr_cke_cntrl,
ddr_csb_cntrl      => ddr_csb_cntrl,
ddr_ODT_cntrl      => ddr_ODT_cntrl,
dqs_enable         => dqs_enable,
dqs_reset          => dqs_reset,
write_enable       => write_enable,
rst_calib          => rst_calib,
rst_dqs_div_int    => rst_dqs_div_int,
cmd_ack            => user_cmd_ack,
init               => init_val,
ar_done            => ar_done
);
data_path0	:	data_path_16bit_rl	port	map	( 
user_input_data     => user_input_data,
user_data_mask      => user_data_mask,
clk	=>	clk_int, 
clk90	=>	clk90_int, 
  clk180 => clk180,
  clk270 => clk270,
reset               => sys_rst,
reset90             => sys_rst90,
reset180            => sys_rst180,
reset270            => sys_rst270,
write_enable        => write_enable,
rst_dqs_div         => dqs_div_rst,
delay_sel           => delay_sel,
dqs_int_delay_in0   => dqs_int_delay_in0,
dqs_int_delay_in1   => dqs_int_delay_in1,
dq                  => dq,      
u_data_val          => user_data_valid,
user_output_data    => user_output_data,
write_en_val        => write_en_val,
reset270_r_val      => reset270_r,
data_mask_f         => data_mask_f,
data_mask_r         => data_mask_r,
write_data_falling  => write_data_falling,
write_data_rising   => write_data_rising                     
);                           
infrastructure0 : infrastructure port map
(
sys_rst            =>   sys_rst,
clk_int            =>   clk_int,
rst_calib1         =>   rst_calib,
delay_sel_val      =>   delay_sel_val,
delay_sel_val1_val =>   delay_sel
); 
iobs0	:	iobs_16bit_00	port	map 
(
clk	=>	clk_int, 
clk90	=>	clk90_int, 
  clk180 => clk180,
  clk270 => clk270,
ddr_rasb_cntrl     =>  ddr_rasb_cntrl, 
ddr_casb_cntrl     =>  ddr_casb_cntrl,
ddr_web_cntrl      =>  ddr_web_cntrl,
ddr_cke_cntrl      =>  ddr_cke_cntrl,
ddr_csb_cntrl      =>  ddr_csb_cntrl,
ddr_address_cntrl  =>  ddr_address_cntrl,
ddr_ba_cntrl       =>  ddr_ba_cntrl,
ddr_ODT_cntrl      =>  ddr_ODT_cntrl,
rst_dqs_div_int    =>  rst_dqs_div_int,
dqs_reset          =>  dqs_reset,
dqs_enable         =>  dqs_enable,
ddr_dqs            =>  ddr_dqs,
ddr_dqs_n          =>  ddr_dqs_n,
ddr_dq             =>  ddr_dq,
write_data_falling =>  write_data_falling, 
write_data_rising  =>  write_data_rising,
write_en_val       =>  write_en_val,
reset270_r         =>  reset270_r,
data_mask_f        =>  data_mask_f, 
data_mask_r        =>  data_mask_r,
ddr_ODT0           =>  ddr_ODT0, 
ddr2_clk0          =>  ddr2_clk0, 
ddr2_clk0b         =>  ddr2_clk0b,
ddr_rasb           =>  ddr_rasb, 
ddr_casb           =>  ddr_casb,
ddr_web            =>  ddr_web,
ddr_ba             =>  ddr_ba,
ddr_address        =>  ddr_address,
ddr_cke            =>  ddr_cke,
ddr_csb            =>  ddr_csb,
rst_dqs_div        =>  dqs_div_rst,
rst_dqs_div_in	    =>  rst_dqs_div_in,
rst_dqs_div_out    =>  rst_dqs_div_out,  
dqs_int_delay_in0  =>  dqs_int_delay_in0,
dqs_int_delay_in1  =>  dqs_int_delay_in1,
dq                 =>  dq,
ddr_dm             =>  ddr_dm
);							
end   arc_ddr2_top_16bit_00;  
