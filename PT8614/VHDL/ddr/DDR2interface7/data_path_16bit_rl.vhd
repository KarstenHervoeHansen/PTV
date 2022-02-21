--******************************************************************************
--
--  Xilinx, Inc. 2002                 www.xilinx.com
--
--
--*******************************************************************************
--
--  File name :       data_path.vhd
--
--  Description :     This module comprises the write and read data paths for the
--                    DDR1 memory interface. The write data along with write enable 
--                    signals are forwarded to the DDR IOB FFs. The read data is 
--                    captured in CLB FFs and finally input to FIFOs.
-- 
--                    
--  Date - revision : 10/16/2003
--
--  Author :          Maria George (Modified by Sailaja)
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
--library synplify; 
--use synplify.attributes.all;
use work.parameter_16bit_00.all;
-- pragma translate_off
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--
entity   data_path_16bit_rl  is
port(
     user_input_data    : in std_logic_vector(31 downto 0);
user_data_mask      : in std_logic_vector(((mask_width)-1) downto 0);
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
     data_mask_f        : out std_logic_vector(1 downto 0);
     data_mask_r        : out std_logic_vector(1 downto 0);
     write_data_falling : out std_logic_vector(15 downto 0);
     write_data_rising  : out std_logic_vector(15 downto 0)
     );
end   data_path_16bit_rl;  

architecture   arc_data_path_16bit_rl of   data_path_16bit_rl    is

component	data_read_16bit_rl 
port(
     clk                : in std_logic;
     clk90              : in std_logic;
     reset90_r          : in std_logic;
     reset270_r         : in std_logic;
     dq                 : in std_logic_vector(15 downto 0);  
     read_valid_data_1  : in std_logic;
     read_valid_data_2  : in std_logic; 
     transfer_done_0    : in std_logic_vector(3 downto 0);
     transfer_done_1    : in std_logic_vector(3 downto 0);
     fifo_00_wr_addr    : in std_logic_vector(3 downto 0);
     fifo_01_wr_addr    : in std_logic_vector(3 downto 0);
     fifo_02_wr_addr    : in std_logic_vector(3 downto 0);
     fifo_03_wr_addr    : in std_logic_vector(3 downto 0);
     fifo_10_wr_addr    : in std_logic_vector(3 downto 0);
     fifo_11_wr_addr    : in std_logic_vector(3 downto 0);
     fifo_12_wr_addr    : in std_logic_vector(3 downto 0);
     fifo_13_wr_addr    : in std_logic_vector(3 downto 0);
     dqs_delayed_col0   : in std_logic_vector(1 downto 0);
     dqs_delayed_col1   : in std_logic_vector(1 downto 0); 
     dqs_div_col0       : in std_logic_vector(1 downto 0);
     dqs_div_col1       : in std_logic_vector(1 downto 0); 
     fifo_00_rd_addr    : in std_logic_vector(3 downto 0);
     fifo_01_rd_addr    : in std_logic_vector(3 downto 0);
     fifo_02_rd_addr    : in std_logic_vector(3 downto 0);
     fifo_03_rd_addr    : in std_logic_vector(3 downto 0);
     fifo_10_rd_addr    : in std_logic_vector(3 downto 0);
     fifo_11_rd_addr    : in std_logic_vector(3 downto 0);
     fifo_12_rd_addr    : in std_logic_vector(3 downto 0);
     fifo_13_rd_addr    : in std_logic_vector(3 downto 0);
     next_state_val     : out std_logic;     
     user_output_data   : out std_logic_vector(31 downto 0)
    );
end component;

component	data_read_controller_16bit_rl 
port(
     clk                : in std_logic;
     clk90              : in std_logic;
  clk180 : in std_logic;
  clk270 : in std_logic;
     reset_r            : in std_logic;
     reset90_r          : in std_logic;
     reset180_r         : in std_logic;
     reset270_r         : in std_logic;
     rst_dqs_div        : in std_logic;
     delay_sel          : in std_logic_vector(4 downto 0);   
     dqs_int_delay_in0  : in std_logic;
     dqs_int_delay_in1  : in std_logic;
     next_state         : in std_logic;          
     u_data_val         : out std_logic;
     read_valid_data_1_val  : out std_logic;
     read_valid_data_2_val  : out std_logic;
     transfer_done_0_val    : out std_logic_vector(3 downto 0);
     transfer_done_1_val    : out std_logic_vector(3 downto 0);
     fifo_00_wr_addr_val    : out std_logic_vector(3 downto 0);
     fifo_01_wr_addr_val    : out std_logic_vector(3 downto 0);
     fifo_02_wr_addr_val    : out std_logic_vector(3 downto 0);
     fifo_03_wr_addr_val    : out std_logic_vector(3 downto 0);
     fifo_10_wr_addr_val    : out std_logic_vector(3 downto 0);
     fifo_11_wr_addr_val    : out std_logic_vector(3 downto 0);
     fifo_12_wr_addr_val    : out std_logic_vector(3 downto 0);
     fifo_13_wr_addr_val    : out std_logic_vector(3 downto 0);
     fifo_00_rd_addr_val    : out std_logic_vector(3 downto 0);
     fifo_01_rd_addr_val    : out std_logic_vector(3 downto 0);    
     fifo_02_rd_addr_val    : out std_logic_vector(3 downto 0);
     fifo_03_rd_addr_val    : out std_logic_vector(3 downto 0);    
     fifo_10_rd_addr_val    : out std_logic_vector(3 downto 0);
     fifo_11_rd_addr_val    : out std_logic_vector(3 downto 0);    
     fifo_12_rd_addr_val    : out std_logic_vector(3 downto 0);
     fifo_13_rd_addr_val    : out std_logic_vector(3 downto 0);    
     dqs_delayed_col0_val   : out std_logic_vector(1 downto 0);
     dqs_delayed_col1_val   : out std_logic_vector(1 downto 0); 
     dqs_div_col0_val       : out std_logic_vector(1 downto 0);
     dqs_div_col1_val       : out std_logic_vector(1 downto 0)
    );
end component;

component	data_write_16bit 
port(
     user_input_data    : in std_logic_vector(31 downto 0);
user_data_mask      : in std_logic_vector(((mask_width)-1) downto 0);
     clk90              : in std_logic;
  clk270 : in std_logic;
    reset90_r          : in std_logic;
     reset270_r         : in std_logic;
     write_enable       : in std_logic;
     write_en_val       : out std_logic;
     write_data_falling : out std_logic_vector(15 downto 0);
     write_data_rising  : out std_logic_vector(15 downto 0);
     data_mask_f        : out std_logic_vector(1 downto 0);
     data_mask_r        : out std_logic_vector(1 downto 0)
     );
end component;   

component data_path_rst
port(
     clk                : in std_logic;
     clk90              : in std_logic;
  clk180 : in std_logic;
  clk270 : in std_logic;
     reset              : in std_logic;
     reset90            : in std_logic;
     reset180           : in std_logic;
     reset270           : in std_logic;
     reset_r            : out std_logic;
     reset90_r          : out std_logic;
     reset180_r         : out std_logic;
     reset270_r         : out std_logic
    );
end component;

signal reset_r          : std_logic;
signal reset90_r        : std_logic;
signal reset180_r       : std_logic;
signal reset270_r       : std_logic;
                                          
                                 
 signal fifo_00_rd_addr    : std_logic_vector(3 downto 0);
 signal fifo_01_rd_addr    : std_logic_vector(3 downto 0);
 signal fifo_02_rd_addr    : std_logic_vector(3 downto 0);
 signal fifo_03_rd_addr    : std_logic_vector(3 downto 0);
 signal fifo_10_rd_addr    : std_logic_vector(3 downto 0);
 signal fifo_11_rd_addr    : std_logic_vector(3 downto 0);
 signal fifo_12_rd_addr    : std_logic_vector(3 downto 0);
 signal fifo_13_rd_addr    : std_logic_vector(3 downto 0);
 signal read_valid_data_1  : std_logic;
 signal read_valid_data_2  : std_logic;
 signal transfer_done_0    : std_logic_vector(3 downto 0);           
 signal transfer_done_1    : std_logic_vector(3 downto 0);           
 signal fifo_00_wr_addr    : std_logic_vector(3 downto 0);           
 signal fifo_01_wr_addr    : std_logic_vector(3 downto 0);           
 signal fifo_02_wr_addr    : std_logic_vector(3 downto 0);           
 signal fifo_03_wr_addr    : std_logic_vector(3 downto 0);           
 signal fifo_10_wr_addr    : std_logic_vector(3 downto 0);          
 signal fifo_11_wr_addr    : std_logic_vector(3 downto 0);             
 signal fifo_12_wr_addr    : std_logic_vector(3 downto 0);           
 signal fifo_13_wr_addr    : std_logic_vector(3 downto 0);           
 signal dqs_delayed_col0   : std_logic_vector(1 downto 0);
 signal dqs_delayed_col1   : std_logic_vector(1 downto 0);
 signal dqs_div_col0       : std_logic_vector(1 downto 0);
 signal dqs_div_col1       : std_logic_vector(1 downto 0);
 signal next_state         : std_logic;

 begin

reset270_r_val <= reset270_r;

data_read0	:	data_read_16bit_rl 
port map (
           clk                 =>    clk,          
           clk90               =>    clk90,
           reset90_r           =>    reset90_r,
           reset270_r          =>    reset270_r,
           dq                  =>    dq,
           read_valid_data_1   =>    read_valid_data_1,
           read_valid_data_2   =>    read_valid_data_2,
           transfer_done_0     =>    transfer_done_0,
           transfer_done_1     =>    transfer_done_1,
           fifo_00_wr_addr     =>    fifo_00_wr_addr,
           fifo_01_wr_addr     =>    fifo_01_wr_addr,
           fifo_02_wr_addr     =>    fifo_02_wr_addr,
           fifo_03_wr_addr     =>    fifo_03_wr_addr,
           fifo_10_wr_addr     =>    fifo_10_wr_addr,
           fifo_11_wr_addr     =>    fifo_11_wr_addr,
           fifo_12_wr_addr     =>    fifo_12_wr_addr,
           fifo_13_wr_addr     =>    fifo_13_wr_addr,
           dqs_delayed_col0    =>    dqs_delayed_col0,
           dqs_delayed_col1    =>    dqs_delayed_col1,
           dqs_div_col0        =>    dqs_div_col0,
           dqs_div_col1        =>    dqs_div_col1,   
           fifo_00_rd_addr     =>    fifo_00_rd_addr,
           fifo_01_rd_addr     =>    fifo_01_rd_addr,
           fifo_02_rd_addr     =>    fifo_02_rd_addr,
           fifo_03_rd_addr     =>    fifo_03_rd_addr,
           fifo_10_rd_addr     =>    fifo_10_rd_addr,
           fifo_11_rd_addr     =>    fifo_11_rd_addr,
           fifo_12_rd_addr     =>    fifo_12_rd_addr,
           fifo_13_rd_addr     =>    fifo_13_rd_addr,
           user_output_data    =>    user_output_data,
           next_state_val      =>    next_state
         );


data_read_controller0	:	data_read_controller_16bit_rl 
port map (
            clk                =>   clk,
            clk90              =>   clk90,
  clk180  =>   clk180,
  clk270  =>   clk270,
            reset_r            =>   reset_r,
            reset90_r          =>   reset90_r,
            reset180_r         =>   reset180_r,
            reset270_r         =>   reset270_r,
            rst_dqs_div        =>   rst_dqs_div,
            delay_sel          =>   delay_sel,
            dqs_int_delay_in0  =>   dqs_int_delay_in0,
            dqs_int_delay_in1  =>   dqs_int_delay_in1,
            next_state             =>    next_state,
            fifo_00_rd_addr_val    =>    fifo_00_rd_addr,
            fifo_01_rd_addr_val    =>    fifo_01_rd_addr,
            fifo_02_rd_addr_val    =>    fifo_02_rd_addr,
            fifo_03_rd_addr_val    =>    fifo_03_rd_addr,
            fifo_10_rd_addr_val    =>    fifo_10_rd_addr,
            fifo_11_rd_addr_val    =>    fifo_11_rd_addr,
            fifo_12_rd_addr_val    =>    fifo_12_rd_addr,
            fifo_13_rd_addr_val    =>    fifo_13_rd_addr,
            u_data_val             =>   u_data_val,
            read_valid_data_1_val  =>   read_valid_data_1,
            read_valid_data_2_val  =>   read_valid_data_2,
            transfer_done_0_val    =>   transfer_done_0,
            transfer_done_1_val    =>   transfer_done_1,
            fifo_00_wr_addr_val    =>   fifo_00_wr_addr,
            fifo_01_wr_addr_val    =>   fifo_01_wr_addr,
            fifo_02_wr_addr_val    =>   fifo_02_wr_addr,
            fifo_03_wr_addr_val    =>   fifo_03_wr_addr,
            fifo_10_wr_addr_val    =>   fifo_10_wr_addr,
            fifo_11_wr_addr_val    =>   fifo_11_wr_addr,
            fifo_12_wr_addr_val    =>   fifo_12_wr_addr,
            fifo_13_wr_addr_val    =>   fifo_13_wr_addr,
            dqs_delayed_col0_val   =>   dqs_delayed_col0,
            dqs_delayed_col1_val   =>   dqs_delayed_col1,
            dqs_div_col0_val       =>   dqs_div_col0,
            dqs_div_col1_val       =>   dqs_div_col1
         );

         
data_write0	:	data_write_16bit 
port map (
          user_input_data    =>   user_input_data,
          user_data_mask     =>   user_data_mask, 
          clk90              =>   clk90,
  clk270  =>   clk270,
          reset90_r          =>   reset90_r,
          reset270_r         =>   reset270_r,
          write_enable       =>   write_enable,
          write_en_val       =>   write_en_val,
          write_data_falling =>   write_data_falling,
          write_data_rising  =>   write_data_rising,
          data_mask_f        =>   data_mask_f,
          data_mask_r        =>   data_mask_r
         );

data_path_rst0 : data_path_rst 
port map (
          clk                =>   clk,            
          clk90              =>   clk90,
  clk180  =>   clk180,
  clk270  =>   clk270,
          reset              =>   reset,
          reset90            =>   reset90,
          reset180           =>   reset180,
          reset270           =>   reset270,
          reset_r            =>   reset_r,
          reset90_r          =>   reset90_r,
          reset180_r         =>   reset180_r,
          reset270_r         =>   reset270_r
         );
                                                                       
         
end   arc_data_path_16bit_rl;  

























































































































































































































































































































































































































































































































































































































































































































































