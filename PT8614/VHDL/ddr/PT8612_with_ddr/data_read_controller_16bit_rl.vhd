--******************************************************************************
--
--  Xilinx, Inc. 2002                 www.xilinx.com
--
--
--*******************************************************************************
--
--  File name :       data_read_controller.vhd
--
--  Description :     This module generates all the control signals  for the 
--                     read data path.
-- 
--                    
--  Date - revision : 10/16/2003
--
--  Author :          Maria George ( Modified by Sailaja)
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
entity   data_read_controller_16bit_rl  is
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
end   data_read_controller_16bit_rl;  

architecture   arc_data_read_controller_16bit_rl of   data_read_controller_16bit_rl    is
 
 

component dqs_delay                                             
              port (
		    clk_in   : in std_logic;
		    sel_in   : in std_logic_vector(4 downto 0);
		    clk_out  : out std_logic
		  );                                           
end component;

component ddr2_dqs_div
  port (
        dqs           : in std_logic;  -- first column for negative edge data
        dqs1          : in std_logic;  -- second column for positive edge data
        reset         : in std_logic; 
        rst_dqs_div_delayed   : in std_logic; 
        dqs_divn      : out std_logic;
        dqs_divp      : out std_logic
       );
end component;

component ddr2_transfer_done
  port (
        clk0            : in std_logic;
        clk90           : in std_logic;                 
  clk180 : in std_logic;
  clk270 : in std_logic;
        reset           : in std_logic;
        reset90         : in std_logic;
        reset180        : in std_logic;
        reset270        : in std_logic;        
        dqs_div         : in std_logic;
        transfer_done0  : out std_logic;
        transfer_done1  : out std_logic;
        transfer_done2  : out std_logic;
        transfer_done3  : out std_logic
       );
end component; 


signal fifo_01_not_empty      : std_logic;
signal fifo_03_not_empty      : std_logic;

signal fifo_01_not_empty_r    : std_logic;
signal fifo_03_not_empty_r    : std_logic;
signal fifo_01_not_empty_r1    : std_logic;
signal fifo_03_not_empty_r1    : std_logic;
signal read_valid_data_1      : std_logic;
signal read_valid_data_2      : std_logic;
signal read_valid_data        : std_logic;
signal rd_data_valid          : std_logic;
signal rst_dqs_div_int        : std_logic;

 signal     transfer_done_0    : std_logic_vector(3 downto 0); 
 signal     transfer_done_1    : std_logic_vector(3 downto 0); 
 signal     fifo_00_wr_addr    : std_logic_vector(3 downto 0); 
 signal     fifo_01_wr_addr    : std_logic_vector(3 downto 0); 
 signal     fifo_02_wr_addr    : std_logic_vector(3 downto 0); 
 signal     fifo_03_wr_addr    : std_logic_vector(3 downto 0); 
 signal     fifo_10_wr_addr    : std_logic_vector(3 downto 0); 
 signal     fifo_11_wr_addr    : std_logic_vector(3 downto 0); 
 signal     fifo_12_wr_addr    : std_logic_vector(3 downto 0); 
 signal     fifo_13_wr_addr    : std_logic_vector(3 downto 0); 
 signal     dqs_delayed_col0   : std_logic_vector(1 downto 0); 
 signal     dqs_delayed_col1   : std_logic_vector(1 downto 0);  
 signal     dqs_div_col0       : std_logic_vector(1 downto 0); 
 signal     dqs_div_col1       : std_logic_vector(1 downto 0);   
 signal     rst_dqs_div_delayed: std_logic;
 signal     fifo_00_rd_addr    : std_logic_vector(3 downto 0);
 signal     fifo_01_rd_addr    : std_logic_vector(3 downto 0);
 signal     fifo_02_rd_addr    : std_logic_vector(3 downto 0);
 signal     fifo_03_rd_addr    : std_logic_vector(3 downto 0);
 signal     fifo_10_rd_addr    : std_logic_vector(3 downto 0);
 signal     fifo_11_rd_addr    : std_logic_vector(3 downto 0);
 signal     fifo_12_rd_addr    : std_logic_vector(3 downto 0);
 signal     fifo_13_rd_addr    : std_logic_vector(3 downto 0);
 signal     rd_data_valid_reg  : std_logic;
 signal     rd_data_valid_1    : std_logic;
 signal     rd_data_valid_2    : std_logic;


 begin

 transfer_done_0_val <= transfer_done_0; 
 transfer_done_1_val <= transfer_done_1; 
 fifo_00_wr_addr_val <= fifo_00_wr_addr; 
 fifo_01_wr_addr_val <= fifo_01_wr_addr; 
 fifo_02_wr_addr_val <= fifo_02_wr_addr; 
 fifo_03_wr_addr_val <= fifo_03_wr_addr; 
 fifo_10_wr_addr_val <= fifo_10_wr_addr; 
 fifo_11_wr_addr_val <= fifo_11_wr_addr; 
 fifo_12_wr_addr_val <= fifo_12_wr_addr; 
 fifo_13_wr_addr_val <= fifo_13_wr_addr; 
 dqs_delayed_col0_val <= dqs_delayed_col0; 
 dqs_delayed_col1_val <= dqs_delayed_col1; 
 dqs_div_col0_val <= dqs_div_col0; 
 dqs_div_col1_val <= dqs_div_col1; 
 
fifo_00_rd_addr_val <= fifo_00_rd_addr;
fifo_01_rd_addr_val <= fifo_01_rd_addr;
fifo_02_rd_addr_val <= fifo_02_rd_addr;
fifo_03_rd_addr_val <= fifo_03_rd_addr;
fifo_10_rd_addr_val <= fifo_10_rd_addr;
fifo_11_rd_addr_val <= fifo_11_rd_addr;
fifo_12_rd_addr_val <= fifo_12_rd_addr;
fifo_13_rd_addr_val <= fifo_13_rd_addr;


process(clk90)
begin
if clk90'event and clk90 = '1' then
 if reset90_r = '1' then
    fifo_00_rd_addr  <= "0000";
    fifo_01_rd_addr  <= "0000";
    fifo_02_rd_addr  <= "0000";
    fifo_03_rd_addr  <= "0000";
    fifo_10_rd_addr  <= "0000";
    fifo_11_rd_addr  <= "0000";
    fifo_12_rd_addr  <= "0000";
    fifo_13_rd_addr  <= "0000";
 else
    case next_state is
      
         when '0' => 
--             if (read_valid_data_1 = '1') then  
             if (rd_data_valid_1 = '1') then  
               fifo_00_rd_addr <= fifo_00_rd_addr + "0001";
               fifo_01_rd_addr <= fifo_01_rd_addr + "0001";
               fifo_10_rd_addr <= fifo_10_rd_addr + "0001";
               fifo_11_rd_addr <= fifo_11_rd_addr + "0001";
             else 
               fifo_00_rd_addr <= fifo_00_rd_addr;
               fifo_01_rd_addr <= fifo_01_rd_addr;
               fifo_10_rd_addr <= fifo_10_rd_addr;
               fifo_11_rd_addr <= fifo_11_rd_addr;
             end if; 
             
         when '1' => 
--             if (read_valid_data_2 = '1') then
             if (rd_data_valid_2 = '1') then
               fifo_02_rd_addr <= fifo_02_rd_addr + "0001";
               fifo_03_rd_addr <= fifo_03_rd_addr + "0001";
               fifo_12_rd_addr <= fifo_12_rd_addr + "0001";
               fifo_13_rd_addr <= fifo_13_rd_addr + "0001";
             else
               fifo_02_rd_addr <= fifo_02_rd_addr;
               fifo_03_rd_addr <= fifo_03_rd_addr;
               fifo_12_rd_addr <= fifo_12_rd_addr;
               fifo_13_rd_addr <= fifo_13_rd_addr;
             end if;
          when others => 
             fifo_00_rd_addr <= "0000";
             fifo_01_rd_addr <= "0000";
             fifo_02_rd_addr <= "0000";
             fifo_03_rd_addr <= "0000";
             fifo_10_rd_addr <= "0000";
             fifo_11_rd_addr <= "0000";
             fifo_12_rd_addr <= "0000";
             fifo_13_rd_addr <= "0000";
     end case;                                             
end if;                                                    
end if;                                                    
end process;  

read_valid_data_1_val <= rd_data_valid_1;
read_valid_data_2_val <= rd_data_valid_2;


--read_valid_data_1_val <= read_valid_data_1;

--read_valid_data_2_val <= read_valid_data_2;



rst_dqs_div_int <= not rst_dqs_div;
 
read_valid_data_1     <= '1' when (fifo_01_not_empty_r1 = '1' and fifo_01_not_empty = '1') else
                         '0';

read_valid_data_2     <= '1' when (fifo_03_not_empty_r1 = '1' and fifo_03_not_empty = '1') else
                         '0';
                         
read_valid_data     <=  read_valid_data_1 or read_valid_data_2;                         

u_data_val          <= rd_data_valid;

fifo_01_not_empty   <= '0' when (fifo_00_rd_addr(3 downto 0) = fifo_01_wr_addr(3 downto 0)) else
                       '1';                                                                     
fifo_03_not_empty   <= '0' when (fifo_02_rd_addr(3 downto 0) = fifo_03_wr_addr(3 downto 0)) else
                       '1';                                                                     


                     
process (clk90)                                            
begin                                                      
  if (rising_edge(clk90)) then                             
    if (reset90_r = '1') then                                 
      fifo_01_not_empty_r   <= '0';
      fifo_03_not_empty_r   <= '0';
      fifo_01_not_empty_r1  <= '0';
      fifo_03_not_empty_r1  <= '0';      
      rd_data_valid         <= '0';       
      rd_data_valid_1       <= '0';                            
      rd_data_valid_2       <= '0';                            
    else                                                   
      fifo_01_not_empty_r   <= fifo_01_not_empty;
      fifo_03_not_empty_r   <= fifo_03_not_empty;
      fifo_01_not_empty_r1  <= fifo_01_not_empty_r;
      fifo_03_not_empty_r1  <= fifo_03_not_empty_r; 
      rd_data_valid_reg     <= read_valid_data;                      
      rd_data_valid         <= rd_data_valid_reg and read_valid_data;                      
--      rd_data_valid         <= read_valid_data;                
      rd_data_valid_1       <=  read_valid_data_1;  --added                          
      rd_data_valid_2       <=  read_valid_data_2; --added                           
    end if;                                                
  end if;                                                  
end process;              
                          
process (clk90)           
begin                     
  if (rising_edge(clk90)) then
    if (reset90_r = '1') then
      fifo_00_wr_addr <= "0000";
    elsif (transfer_done_0(0) = '1') then 
      fifo_00_wr_addr <= fifo_00_wr_addr + "0001";
    end if;
  end if;    
end process;

process (clk90)
begin
  if (rising_edge(clk90)) then
    if (reset90_r = '1') then
      fifo_01_wr_addr <= "0000";
    elsif (transfer_done_0(1) = '1') then 
      fifo_01_wr_addr <= fifo_01_wr_addr + "0001";
    end if;
  end if;    
end process;

process (clk90)
begin
  if (rising_edge(clk90)) then
    if ( reset90_r = '1') then
      fifo_02_wr_addr <= "0000";
    elsif (transfer_done_0(2) = '1') then 
      fifo_02_wr_addr <= fifo_02_wr_addr + "0001";
    end if;
  end if;    
end process;

process (clk90)
begin
  if (rising_edge(clk90)) then
    if ( reset90_r = '1') then
      fifo_03_wr_addr <= "0000";
    elsif (transfer_done_0(3) = '1') then 
      fifo_03_wr_addr <= fifo_03_wr_addr + "0001";
    end if;
  end if;    
end process;
----------------------------------------------------------
process (clk90)
begin
  if (rising_edge(clk90)) then
    if ( reset90_r = '1') then
      fifo_10_wr_addr <= "0000";
    elsif (transfer_done_1(0) = '1') then 
      fifo_10_wr_addr <= fifo_10_wr_addr + "0001";
    end if;
  end if;    
end process;

process (clk90)
begin
  if (rising_edge(clk90)) then
    if ( reset90_r = '1') then
      fifo_11_wr_addr <= "0000";
    elsif (transfer_done_1(1) = '1') then 
      fifo_11_wr_addr <= fifo_11_wr_addr + "0001";
    end if;
  end if;    
end process;

process (clk90)
begin
  if (rising_edge(clk90)) then
    if ( reset90_r = '1') then
      fifo_12_wr_addr <= "0000";
    elsif (transfer_done_1(2) = '1') then 
      fifo_12_wr_addr <= fifo_12_wr_addr + "0001";
    end if;
  end if;    
end process;

process (clk90)
begin
  if (rising_edge(clk90)) then
    if ( reset90_r = '1') then
      fifo_13_wr_addr <= "0000";
    elsif (transfer_done_1(3) = '1') then 
      fifo_13_wr_addr <= fifo_13_wr_addr + "0001";
    end if;
  end if;    
end process;


--***********************************************************************
--    Read Data Capture Module Instantiations
--

                             
-------------------------------------------------------------------------------------------------------------------------------------------------
--**************************************************************************************************
-- rst_dqs_div internal delay to match dqs internal delay
--**************************************************************************************************

rst_dqs_div_delay0 : dqs_delay port map (                                                                          
	                                 clk_in   => rst_dqs_div_int,  --rst_dqs_div, --   
	                                 sel_in   => delay_sel,                                
	                                 clk_out  => rst_dqs_div_delayed                               
	                                 );



--**************************************************************************************************
-- DQS Internal Delay Circuit implemented in LUTs
--**************************************************************************************************

-- Internal Clock Delay circuit placed in the first column (for falling edge data) adjacent to IOBs                               
dqs_delay0_col0 : dqs_delay port map (                                                                          
	                              clk_in   => dqs_int_delay_in0,
	                              sel_in   => delay_sel,                                
	                              clk_out  => dqs_delayed_col0(0)                               
	                             );

-- Internal Clock Delay circuit placed in the second column (for rising edge data) adjacent to IOBs                               	                    
dqs_delay0_col1 : dqs_delay port map (                                                                          
	                              clk_in   => dqs_int_delay_in0,
	                              sel_in   => delay_sel,                                
	                              clk_out  => dqs_delayed_col1(0)                               
	                             );
	                    
-- Internal Clock Delay circuit placed in the first column (for falling edge data) adjacent to IOBs                               
dqs_delay1_col0 : dqs_delay port map (                                                                          
	                              clk_in   => dqs_int_delay_in1,
	                              sel_in   => delay_sel,                                
	                              clk_out  => dqs_delayed_col0(1)                               
	                             );

-- Internal Clock Delay circuit placed in the second column (for rising edge data) adjacent to IOBs                               	                    
dqs_delay1_col1 : dqs_delay port map (                                                                          
	                              clk_in   => dqs_int_delay_in1,
	                              sel_in   => delay_sel,                                
	                              clk_out  => dqs_delayed_col1(1)                               
	                             );
	                    
	                    
-------------------------------------------------------------------------------------------------------------------------------------------------- 
--***************************************************************************************************
-- DQS Divide by 2 instantiations
--*************************************************************************************************** 

ddr2_dqs_div0 : ddr2_dqs_div
port map (        
          dqs           => dqs_delayed_col0(0),
          dqs1          => dqs_delayed_col1(0), 
          reset         => reset_r,
          rst_dqs_div_delayed   => rst_dqs_div_delayed,
          dqs_divn      => dqs_div_col0(0),
          dqs_divp      => dqs_div_col1(0)
         );
         
ddr2_dqs_div1 : ddr2_dqs_div
port map (     
          dqs           => dqs_delayed_col0(1),
          dqs1          => dqs_delayed_col1(1),          
          reset         => reset_r,
          rst_dqs_div_delayed   => rst_dqs_div_delayed,
          dqs_divn      => dqs_div_col0(1),
          dqs_divp      => dqs_div_col1(1)
         );
         
--------------------------------------------------------------------------------------------------------------------------------------------
--****************************************************************************************************************
-- Transfer done instantiations (One instantiation peer strobe)
--****************************************************************************************************************         
        
ddr2_transfer_done0 : ddr2_transfer_done
  port map (
            clk0            => clk,
            clk90           => clk90,
   clk180 => clk180,
   clk270 => clk270,
            reset           => reset_r,
            reset90         => reset90_r,
            reset180        => reset180_r,
            reset270        => reset270_r,            
            dqs_div         => dqs_div_col1(0),
            transfer_done0  => transfer_done_0(0),
            transfer_done1  => transfer_done_0(1),
            transfer_done2  => transfer_done_0(2),
            transfer_done3  => transfer_done_0(3)
           );

ddr2_transfer_done1 : ddr2_transfer_done
  port map (
            clk0            => clk,
            clk90           => clk90,
   clk180 => clk180,
   clk270 => clk270,
            reset           => reset_r,
            reset90         => reset90_r,
            reset180        => reset180_r,
            reset270        => reset270_r,
            dqs_div         => dqs_div_col1(1),
            transfer_done0  => transfer_done_1(0),
            transfer_done1  => transfer_done_1(1),
            transfer_done2  => transfer_done_1(2),
            transfer_done3  => transfer_done_1(3)
           );
           
                                                                                           
end   arc_data_read_controller_16bit_rl;  

























































































































































































































































































































































































































































































































































































































































































































































