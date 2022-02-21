library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;

entity period_quad_count is
   generic(
   This component is not working!
      count_val1 : integer;            -- must be >= 2
      count_val2 : integer;            -- must be >= 2
      count_val3 : integer;            -- must be >= 2
      count_val4 : integer;            -- must be >= 2
      bits1      : integer;
      bits2      : integer;
      bits3      : integer;
      bits4      : integer
   );
   Port(
      mreset_i    : in std_logic;
      clk_i       : in std_logic;
      sync_reset_i: in std_logic;
      sync_o      : out std_logic
   );
end period_quad_count;

architecture Behavioral of period_quad_count is

signal count1      : std_logic_vector(bits1-1 downto 0);
signal count2      : std_logic_vector(bits2-1 downto 0);
signal count3      : std_logic_vector(bits1-1 downto 0);
signal count4      : std_logic_vector(bits2-1 downto 0);
signal count1_zero : std_logic;
signal count2_zero : std_logic;
signal count3_zero : std_logic;
signal count4_zero : std_logic;


begin

--------------------------------------------------------------
-- Counter 1
--------------------------------------------------------------
counting_1 : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                          
   count1 <= conv_std_logic_vector(count_val1,bits1);
   count1_zero <= '0';
elsif clk_i'event and clk_i = '1' then
   if sync_reset_i = '1' then
      count1 <= conv_std_logic_vector(count_val1-2,bits1);
      count1_zero <= '0';
   else
      if count1 = conv_std_logic_vector(0,bits1) then
         count1 <= conv_std_logic_vector(count_val1-1,bits1);
         count1_zero <= '1';
      else
         count1 <= count1 - 1;
         count1_zero <= '0';
      end if;
   end if;
end if;    
end process;


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