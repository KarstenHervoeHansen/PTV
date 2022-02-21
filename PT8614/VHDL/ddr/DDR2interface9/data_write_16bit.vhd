--******************************************************************************
--
--  Xilinx, Inc. 2002                 www.xilinx.com
--
--
--*******************************************************************************
--
--  File name :       data_write.vhd
--
--  Description :     This module comprises the write data paths for the
--                    DDR1 memory interface.                 
-- 
--                    
--  Date - revision : 10/16/2003
--
--  Author :          Maria George (Modifed by Sailaja)
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
--use work.parameter_16bit.all;

entity   data_write_16bit  is
port(
     user_input_data    : in std_logic_vector(31 downto 0);
     user_data_mask     : in std_logic_vector(3 downto 0);
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
end   data_write_16bit;  

architecture   arc_data_write_16bit of   data_write_16bit    is
component FD
   port(
      Q                              :  out   STD_LOGIC;
      D                              :  in    STD_LOGIC;
      C                              :  in    STD_LOGIC);
end component;

signal write_en_P1            : std_logic; -- write enable Pipeline stage
signal write_en_P2            : std_logic;
signal write_en_P3            : std_logic;
signal write_en_int           : std_logic;
signal write_data             : std_logic_vector(31 downto 0);
signal write_data1            : std_logic_vector(31 downto 0);
signal write_data2            : std_logic_vector(31 downto 0);
signal write_data3            : std_logic_vector(31 downto 0);
signal write_data4            : std_logic_vector(31 downto 0);
signal write_data5            : std_logic_vector(31 downto 0);
signal write_data6            : std_logic_vector(31 downto 0);
signal write_data7            : std_logic_vector(31 downto 0);
signal write_data_int         : std_logic_vector(31 downto 0);
signal write_data270_1        : std_logic_vector(15 downto 0);
signal write_data270_2        : std_logic_vector(15 downto 0);
signal write_data270_3        : std_logic_vector(15 downto 0);



signal write_data_m    : std_logic_vector (3 downto 0);    
signal write_data_m1   : std_logic_vector (3 downto 0);
signal write_data_m2   : std_logic_vector (3 downto 0);
signal write_data_m3   : std_logic_vector (3 downto 0);
signal write_data_m4   : std_logic_vector (3 downto 0);
signal write_data_m5   : std_logic_vector (3 downto 0);
signal write_data_m6   : std_logic_vector (3 downto 0);
signal write_data_m7   : std_logic_vector (3 downto 0);
signal write_data_mask : std_logic_vector (3 downto 0);

signal write_data_m270_1 : std_logic_vector (1 downto 0);
signal write_data_m270_2 : std_logic_vector (1 downto 0);
signal write_data_m270_3 : std_logic_vector (1 downto 0);



attribute syn_preserve : boolean;
attribute syn_preserve of write_data  : signal is true;
attribute syn_preserve of write_data1 : signal is true;
attribute syn_preserve of write_data2 : signal is true;
attribute syn_preserve of write_data3 : signal is true;
attribute syn_preserve of write_data4 : signal is true;
attribute syn_preserve of write_data5 : signal is true;
attribute syn_preserve of write_data6 : signal is true;
attribute syn_preserve of write_data7 : signal is true;

attribute syn_preserve of write_data270_1 : signal is true;
attribute syn_preserve of write_data270_2 : signal is true;
attribute syn_preserve of write_data270_3 : signal is true;


 begin
                                                                     


--data_mask_f <= "00";
--data_mask_r <= "00";

  process(clk90)
         begin
         if clk90'event and clk90 = '1' then  
        write_data_int    <=  user_input_data;
                                write_data_m    <= user_data_mask;   
        write_data1 <= write_data_int;
                                               write_data_m1 <= write_data_m;     
        write_data2 <= write_data1;
                                            write_data_m2 <= write_data_m1; 
        write_data3 <= write_data2;
                                      write_data_m3 <= write_data_m2; 
 end if;
          end process;  
INST_DATA_PATHS1: for I in 31 downto 0 generate
begin
write_data_mo  : FD  PORT MAP (
                Q => write_data4(i),
                D => write_data3 (i),
                C => clk90
        );

end generate;

INST_DATA_PATHS2: for I in 3 downto 0 generate
begin
write_data_mas  : FD  PORT MAP(
                Q => write_data_m4(i),
                D => write_data_m3 (i),
                C =>  clk90);

              end generate;

 process(clk90)
         begin
         if clk90'event and clk90 = '1' then   
         write_data5    <=   write_data4 ;
                          write_data_m5    <=   write_data_m4 ;          

        write_data   <=   write_data5;
        write_data_mask <= write_data_m5;
         end if;
          end process;    -- pipeline varables

--process(clk90)
process(clk270)
begin
--  if clk90'event and clk90 = '0' then
  if clk270'event and clk270 = '1' then
write_data270_1  <= write_data3 (31 downto 16) ;
write_data_m270_1 <= write_data_m3 (3 downto 2);
write_data270_2 <= write_data270_1; 
write_data_m270_2 <= write_data_m270_1; 
write_data270_3 <= write_data270_2; 
write_data_m270_3 <= write_data_m270_2; 
-- varable_in

  end if;
end process;

-- assign statements--

write_data_rising  <= write_data270_3;
write_data_falling <= write_data(15 downto 0);

data_mask_r      <=   write_data_m270_3;
data_mask_f      <=   write_data_mask(1 downto 0);


-- data path for write enable
process(clk90)
begin
  if clk90'event and clk90 = '1' then 
   if reset90_r = '1' then
    write_en_P1 <= '0';
    write_en_P2 <= '0';
    write_en_P3 <= '0';
   else 
     write_en_P1 <= write_enable;
     write_en_P2 <= write_en_P1;
     write_en_P3 <= write_en_P2;
   end if;  
  end if;
end process;

-- data path for write enable
process(clk90)
begin
 if clk90'event and clk90 = '0' then
  if reset90_r = '1' then
    write_en_int    <= '0';
    write_en_val    <= '0';
    else
     write_en_int   <= write_en_P2;--P2
     write_en_val   <= write_en_P1; --int;
   end if;
 end if;
end process;
                                                      
end   arc_data_write_16bit; 