--******************************************************************************
--
--  Xilinx, Inc. 2002                 www.xilinx.com
--
--
--*******************************************************************************
--
--  File name :       data_read.vhd
--
--  Description :     This module comprises the write and read data paths for the
--                    ddr2 memory interface. The read data is 
--                    captured in CLB FFs and finally input to FIFOs.
-- 
--                    
--  Date - revision : 10/16/2003
--
--  Author :          Maria George (modified by Sailaja)
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
--
-- pragma translate_off
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--
entity   data_read_16bit_rl  is
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
end   data_read_16bit_rl;  

architecture   arc_data_read_16bit_rl of   data_read_16bit_rl    is

component RAM_8D
port(
     DPO   : out std_logic_vector(7 downto 0);
     A0    : in std_logic;
     A1    : in std_logic;
     A2    : in std_logic;
     A3    : in std_logic;
     D     : in std_logic_vector(7 downto 0);
     DPRA0 : in std_logic;
     DPRA1 : in std_logic;
     DPRA2 : in std_logic;
     DPRA3 : in std_logic;
     WCLK  : in std_logic;
     WE    : in std_logic
     );
end component;

component ddr2_dqbit
  port (
        reset               : in std_logic;         
        dqs                 : in std_logic;
        dqs1                : in std_logic;     
        dqs_div_1           : in std_logic;
        dqs_div_2           : in std_logic;
        dq                  : in std_logic;
        fbit_0              : out std_logic;
        fbit_1              : out std_logic;
        fbit_2              : out std_logic;
        fbit_3              : out std_logic
       );
end component;   


signal fbit_0                 : std_logic_vector(15 downto 0);
signal fbit_1                 : std_logic_vector(15 downto 0);
signal fbit_2                 : std_logic_vector(15 downto 0);
signal fbit_3                 : std_logic_vector(15 downto 0);
signal fifo_00_data_out       : std_logic_vector(7 downto 0);
signal fifo_01_data_out       : std_logic_vector(7 downto 0);
signal fifo_02_data_out       : std_logic_vector(7 downto 0);
signal fifo_03_data_out       : std_logic_vector(7 downto 0);
signal fifo_10_data_out       : std_logic_vector(7 downto 0);
signal fifo_11_data_out       : std_logic_vector(7 downto 0);
signal fifo_12_data_out       : std_logic_vector(7 downto 0);
signal fifo_13_data_out       : std_logic_vector(7 downto 0);
signal next_state             : std_logic;


 begin

next_state_val <= next_state;                                          
                        
process(clk90)
begin
if clk90'event and clk90 = '1' then
 if reset90_r = '1' then
    next_state       <= '0';
    user_output_data  <= (others => '0');
 else
    case next_state is
      
         when '0' => 
             if (read_valid_data_1 = '1') then  
               next_state <= '1';
               user_output_data  <= (fifo_10_data_out & fifo_00_data_out &                                    
            	                        fifo_11_data_out & fifo_01_data_out);
             else
               next_state      <= '0';
             end if;              
         when '1' => 
             if (read_valid_data_2 = '1') then
               next_state      <= '0';
               user_output_data <= (fifo_12_data_out & fifo_02_data_out &
                                    fifo_13_data_out & fifo_03_data_out);
             else
               next_state <= '1';
             end if;
         when others => 
             next_state <= '0';
             user_output_data <= (others => '0');

     end case;                                             
end if;                                                    
end if;                                                    
end process;  
                                                           

--------------------------------------------------------------------------------------------------------------------------------
--******************************************************************************************************************************
-- DDR Data bit instantiations (16-bits)
--******************************************************************************************************************************            
           

ddr2_dqbit0 : ddr2_dqbit port map
                              (
                               reset                 => reset270_r,
                               dqs                   => dqs_delayed_col0(0),
                               dqs1                  => dqs_delayed_col1(0),
                               dqs_div_1             => dqs_div_col0(0),
                               dqs_div_2             => dqs_div_col1(0),
                               dq                    => dq(0),
                               fbit_0                => fbit_0(0),
                               fbit_1                => fbit_1(0),
                               fbit_2                => fbit_2(0),
                               fbit_3                => fbit_3(0)
                              );

ddr2_dqbit1 : ddr2_dqbit port map
                              (
                               reset              => reset270_r,
                               dqs                => dqs_delayed_col0(0),
                               dqs1               => dqs_delayed_col1(0),
                               dqs_div_1          => dqs_div_col0(0),
                               dqs_div_2          => dqs_div_col1(0),
                               dq                 => dq(1),
                               fbit_0             => fbit_0(1),
                               fbit_1             => fbit_1(1),
                               fbit_2             => fbit_2(1),
                               fbit_3             => fbit_3(1)
                              );

ddr2_dqbit2 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(0),
                               dqs1               => dqs_delayed_col1(0),
                               dqs_div_1          => dqs_div_col0(0),
                               dqs_div_2          => dqs_div_col1(0),
                               dq                 => dq(2),
                               fbit_0             => fbit_0(2),
                               fbit_1             => fbit_1(2),
                               fbit_2             => fbit_2(2),
                               fbit_3             => fbit_3(2)
                              );

ddr2_dqbit3 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(0),
                               dqs1               => dqs_delayed_col1(0),
                               dqs_div_1          => dqs_div_col0(0),
                               dqs_div_2          => dqs_div_col1(0),
                               dq                 => dq(3),
                               fbit_0             => fbit_0(3),
                               fbit_1             => fbit_1(3),
                               fbit_2             => fbit_2(3),
                               fbit_3             => fbit_3(3)
                              );

ddr2_dqbit4 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(0),
                               dqs1               => dqs_delayed_col1(0),
                               dqs_div_1          => dqs_div_col0(0),
                               dqs_div_2          => dqs_div_col1(0),
                               dq                 => dq(4),
                               fbit_0             => fbit_0(4),
                               fbit_1             => fbit_1(4),
                               fbit_2             => fbit_2(4),
                               fbit_3             => fbit_3(4)
                              );

ddr2_dqbit5 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(0),
                               dqs1               => dqs_delayed_col1(0),
                               dqs_div_1          => dqs_div_col0(0),
                               dqs_div_2          => dqs_div_col1(0),
                               dq                 => dq(5),
                               fbit_0             => fbit_0(5),
                               fbit_1             => fbit_1(5),
                               fbit_2             => fbit_2(5),
                               fbit_3             => fbit_3(5)
                              );

ddr2_dqbit6 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(0),
                               dqs1               => dqs_delayed_col1(0),
                               dqs_div_1          => dqs_div_col0(0),
                               dqs_div_2          => dqs_div_col1(0),
                               dq                 => dq(6),
                               fbit_0             => fbit_0(6),
                               fbit_1             => fbit_1(6),
                               fbit_2             => fbit_2(6),
                               fbit_3             => fbit_3(6)
                              );

ddr2_dqbit7 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(0),
                               dqs1               => dqs_delayed_col1(0),
                               dqs_div_1          => dqs_div_col0(0),
                               dqs_div_2          => dqs_div_col1(0),
                               dq                 => dq(7),
                               fbit_0             => fbit_0(7),
                               fbit_1             => fbit_1(7),
                               fbit_2             => fbit_2(7),
                               fbit_3             => fbit_3(7)
                              );

ddr2_dqbit8 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(1),
                               dqs1               => dqs_delayed_col1(1),
                               dqs_div_1          => dqs_div_col0(1),
                               dqs_div_2          => dqs_div_col1(1),
                               dq                 => dq(8),
                               fbit_0             => fbit_0(8),
                               fbit_1             => fbit_1(8),
                               fbit_2             => fbit_2(8),
                               fbit_3             => fbit_3(8)
                              );

ddr2_dqbit9 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(1),
                               dqs1               => dqs_delayed_col1(1),
                               dqs_div_1          => dqs_div_col0(1),
                               dqs_div_2          => dqs_div_col1(1),
                               dq                 => dq(9),
                               fbit_0             => fbit_0(9),
                               fbit_1             => fbit_1(9),
                               fbit_2             => fbit_2(9),
                               fbit_3             => fbit_3(9)
                              );

ddr2_dqbit10 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(1),
                               dqs1               => dqs_delayed_col1(1),
                               dqs_div_1          => dqs_div_col0(1),
                               dqs_div_2          => dqs_div_col1(1),
                               dq                 => dq(10),
                               fbit_0             => fbit_0(10),
                               fbit_1             => fbit_1(10),
                               fbit_2             => fbit_2(10),
                               fbit_3             => fbit_3(10)
                              );

ddr2_dqbit11 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(1),
                               dqs1               => dqs_delayed_col1(1),
                               dqs_div_1          => dqs_div_col0(1),
                               dqs_div_2          => dqs_div_col1(1),
                               dq                 => dq(11),
                               fbit_0             => fbit_0(11),
                               fbit_1             => fbit_1(11),
                               fbit_2             => fbit_2(11),
                               fbit_3             => fbit_3(11)
                              );

ddr2_dqbit12 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(1),
                               dqs1               => dqs_delayed_col1(1),
                               dqs_div_1          => dqs_div_col0(1),
                               dqs_div_2          => dqs_div_col1(1),
                               dq                 => dq(12),
                               fbit_0             => fbit_0(12),
                               fbit_1             => fbit_1(12),
                               fbit_2             => fbit_2(12),
                               fbit_3             => fbit_3(12)
                              );

ddr2_dqbit13 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(1),
                               dqs1               => dqs_delayed_col1(1),
                               dqs_div_1          => dqs_div_col0(1),
                               dqs_div_2          => dqs_div_col1(1),
                               dq                 => dq(13),
                               fbit_0             => fbit_0(13),
                               fbit_1             => fbit_1(13),
                               fbit_2             => fbit_2(13),
                               fbit_3             => fbit_3(13)
                              );

ddr2_dqbit14 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(1),
                               dqs1               => dqs_delayed_col1(1),
                               dqs_div_1          => dqs_div_col0(1),
                               dqs_div_2          => dqs_div_col1(1),
                               dq                 => dq(14),
                               fbit_0             => fbit_0(14),
                               fbit_1             => fbit_1(14),
                               fbit_2             => fbit_2(14),
                               fbit_3             => fbit_3(14)
                              );

ddr2_dqbit15 : ddr2_dqbit port map
                              (
                               reset              =>  reset270_r,
                               dqs                => dqs_delayed_col0(1),
                               dqs1               => dqs_delayed_col1(1),
                               dqs_div_1          => dqs_div_col0(1),
                               dqs_div_2          => dqs_div_col1(1),
                               dq                 => dq(15),
                               fbit_0             => fbit_0(15),
                               fbit_1             => fbit_1(15),
                               fbit_2             => fbit_2(15),
                               fbit_3             => fbit_3(15)
                              );


--*************************************************************************************************************************
-- Distributed RAM 8 bit wide FIFO instantiations (4 FIFOs per strobe, 1 for each fbit0 through 3)
--*************************************************************************************************************************
-- FIFOs associated with ddr2_dqs(0)
ram_8d_dqs0_fbit0 : RAM_8D                  
port map (                                                   
          DPO    => fifo_00_data_out,          
          A0     => fifo_00_wr_addr(0),          
          A1     => fifo_00_wr_addr(1),
          A2     => fifo_00_wr_addr(2),
          A3     => fifo_00_wr_addr(3),
          D      => fbit_0(7 downto 0),      
          DPRA0  => fifo_00_rd_addr(0),
          DPRA1  => fifo_00_rd_addr(1),
          DPRA2  => fifo_00_rd_addr(2),
          DPRA3  => fifo_00_rd_addr(3),
          WCLK   => clk90,          
          WE     => transfer_done_0(0)
         );                       

ram_8d_dqs0_fbit1 : RAM_8D                  
port map (                                                   
          DPO    => fifo_01_data_out,          
          A0     => fifo_01_wr_addr(0),          
          A1     => fifo_01_wr_addr(1),
          A2     => fifo_01_wr_addr(2),
          A3     => fifo_01_wr_addr(3),
          D      => fbit_1(7 downto 0),      
          DPRA0  => fifo_01_rd_addr(0),
          DPRA1  => fifo_01_rd_addr(1),
          DPRA2  => fifo_01_rd_addr(2),
          DPRA3  => fifo_01_rd_addr(3),
          WCLK   => clk90,          
          WE     => transfer_done_0(1)
         );
         
ram_8d_dqs0_fbit2 : RAM_8D                  
port map (                                                   
          DPO    => fifo_02_data_out,          
          A0     => fifo_02_wr_addr(0),          
          A1     => fifo_02_wr_addr(1),
          A2     => fifo_02_wr_addr(2),
          A3     => fifo_02_wr_addr(3),
          D      => fbit_2(7 downto 0),      
          DPRA0  => fifo_02_rd_addr(0),
          DPRA1  => fifo_02_rd_addr(1),
          DPRA2  => fifo_02_rd_addr(2),
          DPRA3  => fifo_02_rd_addr(3),
          WCLK   => clk90,          
          WE     => transfer_done_0(2)
         ); 
         
ram_8d_dqs0_fbit3 : RAM_8D                  
port map (                                                   
          DPO    => fifo_03_data_out,          
          A0     => fifo_03_wr_addr(0),          
          A1     => fifo_03_wr_addr(1),
          A2     => fifo_03_wr_addr(2),
          A3     => fifo_03_wr_addr(3),
          D      => fbit_3(7 downto 0),      
          DPRA0  => fifo_03_rd_addr(0),
          DPRA1  => fifo_03_rd_addr(1),
          DPRA2  => fifo_03_rd_addr(2),
          DPRA3  => fifo_03_rd_addr(3),
          WCLK   => clk90,          
          WE     => transfer_done_0(3)
         ); 
         
-- FIFOs associated with ddr2_dqs(1)         
         
ram_8d_dqs1_fbit0 : RAM_8D                  
port map (                                                   
          DPO    => fifo_10_data_out,          
          A0     => fifo_10_wr_addr(0),          
          A1     => fifo_10_wr_addr(1),
          A2     => fifo_10_wr_addr(2),
          A3     => fifo_10_wr_addr(3),
          D      => fbit_0(15 downto 8),      
          DPRA0  => fifo_10_rd_addr(0),
          DPRA1  => fifo_10_rd_addr(1),
          DPRA2  => fifo_10_rd_addr(2),
          DPRA3  => fifo_10_rd_addr(3),
          WCLK   => clk90,          
          WE     => transfer_done_1(0)
         );         

ram_8d_dqs1_fbit1 : RAM_8D                  
port map (                                                   
          DPO    => fifo_11_data_out,          
          A0     => fifo_11_wr_addr(0),          
          A1     => fifo_11_wr_addr(1),
          A2     => fifo_11_wr_addr(2),
          A3     => fifo_11_wr_addr(3),
          D      => fbit_1(15 downto 8),      
          DPRA0  => fifo_11_rd_addr(0),
          DPRA1  => fifo_11_rd_addr(1),
          DPRA2  => fifo_11_rd_addr(2),
          DPRA3  => fifo_11_rd_addr(3),
          WCLK   => clk90,          
          WE     => transfer_done_1(1)
         ); 
         
ram_8d_dqs1_fbit2 : RAM_8D                  
port map (                                                   
          DPO    => fifo_12_data_out,          
          A0     => fifo_12_wr_addr(0),          
          A1     => fifo_12_wr_addr(1),
          A2     => fifo_12_wr_addr(2),
          A3     => fifo_12_wr_addr(3),
          D      => fbit_2(15 downto 8),      
          DPRA0  => fifo_12_rd_addr(0),
          DPRA1  => fifo_12_rd_addr(1),
          DPRA2  => fifo_12_rd_addr(2),
          DPRA3  => fifo_12_rd_addr(3),
          WCLK   => clk90,          
          WE     => transfer_done_1(2)
         ); 
         
ram_8d_dqs1_fbit3 : RAM_8D                  
port map (                                                   
          DPO    => fifo_13_data_out,          
          A0     => fifo_13_wr_addr(0),          
          A1     => fifo_13_wr_addr(1),
          A2     => fifo_13_wr_addr(2),
          A3     => fifo_13_wr_addr(3),
          D      => fbit_3(15 downto 8),      
          DPRA0  => fifo_13_rd_addr(0),
          DPRA1  => fifo_13_rd_addr(1),
          DPRA2  => fifo_13_rd_addr(2),
          DPRA3  => fifo_13_rd_addr(3),
          WCLK   => clk90,          
          WE     => transfer_done_1(3)
         );                

         
end   arc_data_read_16bit_rl;  

























































































































































































































































































































































































































































































































































































































































































































































