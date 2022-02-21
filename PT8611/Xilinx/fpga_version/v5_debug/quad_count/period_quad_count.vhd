library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;

entity frame_count is
   Port(
      mreset_i     : in std_logic;
      clk_i        : in std_logic;
      sync_reset_i : in std_logic;
      count_val1   : in std_logic_vector(3 downto 0);
      count_val2   : in std_logic_vector(9 downto 0);
      count_val3   : in std_logic_vector(10 downto 0);
      count_val4   : in std_logic_vector(1 downto 0);                   
      sync_o       : out std_logic
   );
end period_quad_count;

architecture Behavioral of period_quad_count is

signal count1      : std_logic_vector(3 downto 0); 
signal count2      : std_logic_vector(9 downto 0); 
signal count3      : std_logic_vector(10 downto 0);
signal count4      : std_logic_vector(1 downto 0); 
signal count1_done : std_logic;
signal count2_done : std_logic;
signal count3_done : std_logic;
signal count4_done : std_logic;


begin

--------------------------------------------------------------
-- Counter 1
--------------------------------------------------------------
counting_1 : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                          
   count1 <= count1_val;
   count1_done <= '0';
elsif clk_i'event and clk_i = '1' then
   if sync_reset_i = '1' then
      count1 <= count1_val;
      count1_done <= '0';
   else
      if count1 = conv_std_logic_vector(1,4) then
         count1 <= count1_val;
         count1_done <= '1';
      else
         count1 <= count1 - 1;
         count1_done <= '0';
      end if;
   end if;
end if;    
end process;







-------

!Lav evt. all-count signal der efter reset tæller alle countere en ned








--------------------------------------------------------------
-- Counter 2
--------------------------------------------------------------
counting_2 : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                          
   count2 <= conv_std_logic_vector(count_val2-1,bits2);
   count2_zero <= '0';
elsif clk_i'event and clk_i = '1' then
   if sync_reset_i = '1' then
      count2 <= conv_std_logic_vector(count_val2-2,bits1);
      count2_zero <= '0';
   else
      if count1_zero = '1' then
         if count2 = conv_std_logic_vector(0,bits2) then
            count2 <= conv_std_logic_vector(count_val2-1,bits2);
            count2_zero <= '1';
         else
            count2 <= count2 - 1;
            count2_zero <= '0';
         end if;
      end if;
   end if;
end if;    
end process;


--------------------------------------------------------------
-- Counter 3
--------------------------------------------------------------
counting_3 : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                          
   count3 <= conv_std_logic_vector(count_val3-1,bits3);
   count3_zero <= '0';
elsif clk_i'event and clk_i = '1' then
   if sync_reset_i = '1' then
      count3 <= conv_std_logic_vector(count_val3-2,bits1);
      count3_zero <= '0';
   else
      if count1_zero = '1' and count2_zero = '1' then
         if count3 = conv_std_logic_vector(0,bits3) then
            count3 <= conv_std_logic_vector(count_val3-1,bits3);
            count3_zero <= '1';
         else
            count3 <= count3 - 1;
            count3_zero <= '0';
         end if;
      end if;
   end if;
end if;    
end process;


--------------------------------------------------------------
-- Counter 4
--------------------------------------------------------------
counting_4 : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                          
   count4 <= conv_std_logic_vector(count_val4-1,bits4);
   count4_zero <= '0';
elsif clk_i'event and clk_i = '1' then
   if sync_reset_i = '1' then
      count4 <= conv_std_logic_vector(count_val4-2,bits1);
      count4_zero <= '0';
   else
      if count1_zero = '1' and count2_zero = '1' and count3_zero = '1' then
         if count4 = conv_std_logic_vector(0,bits4) then
            count4 <= conv_std_logic_vector(count_val4-1,bits4);
            count4_zero <= '1';
         else
            count4 <= count4 - 1;
            count4_zero <= '0';
         end if;
      end if;
   end if;
end if;    
end process;


--------------------------------------------------------------
-- Generate sync_o pulse
--------------------------------------------------------------
sync_generation : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                        
   sync_o <= '0';
elsif clk_i'event and clk_i = '1' then
   if count1_zero = '1' and count2_zero = '1' and count3_zero = '1' and count4_zero = '1' then
      sync_o <= '1';
   else
      sync_o <= '0';
   end if;
end if;    
end process;


end Behavioral;