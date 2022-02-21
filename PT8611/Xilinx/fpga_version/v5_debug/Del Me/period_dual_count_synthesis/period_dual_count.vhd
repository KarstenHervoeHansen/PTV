library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;

entity period_dual_count is
   generic(
      count_val1    : integer;            -- must be >= 3
      count_val2    : integer;            -- must be >= 3
      bits1         : integer;
      bits2         : integer
   );
   Port(
      mreset_i      : in std_logic;
      clk_i         : in std_logic;
      sync_reset_i  : in std_logic;
      sync_o        : out std_logic
   );
end period_dual_count;

architecture Behavioral of period_dual_count is

signal count1              : std_logic_vector(bits1-1 downto 0);
signal count2              : std_logic_vector(bits2-1 downto 0);
signal count1_cycle        : std_logic;
signal count2_cycle        : std_logic;
signal sync_reset_delayed1 : std_logic;

begin


--------------------------------------------------------------
-- Sync_reset delaying
--------------------------------------------------------------
sync_reset_delaying : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                          
   sync_reset_delayed1 <= '0';
elsif clk_i'event and clk_i = '1' then
   sync_reset_delayed1 <= sync_reset_i;
end if;
end process;


--------------------------------------------------------------
-- Counter 1
--------------------------------------------------------------
counting_1 : process(mreset_i, clk_i)
begin
if mreset_i = '0' then
   count1 <= conv_std_logic_vector(count_val1,bits1);
   count1_cycle <= '0';
elsif clk_i'event and clk_i = '1' then
   if sync_reset_i = '1' then
      count1 <= conv_std_logic_vector(count_val1,bits1);
      count1_cycle <= '0';
   else
      if count1 = conv_std_logic_vector(1,bits1) then
         count1 <= conv_std_logic_vector(count_val1,bits1);
         count1_cycle <= '1';
      else
         count1 <= count1 - 1;
         count1_cycle <= '0';
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
   count2 <= conv_std_logic_vector(count_val2,bits2);
   count2_cycle <= '0';
elsif clk_i'event and clk_i = '1' then
   if sync_reset_i = '1' then
      count2 <= conv_std_logic_vector(count_val2,bits2);
      count2_cycle <= '0';
   else
      if count1_cycle = '1' or sync_reset_delayed1 = '1' then
         if count2 = conv_std_logic_vector(1,bits2) then
            count2 <= conv_std_logic_vector(count_val2,bits2);
            count2_cycle <= '1';
         else
            count2 <= count2 - 1;
            count2_cycle <= '0';
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
   if count1_cycle = '1' and count2_cycle = '1' then
      sync_o <= '1';
   else
      sync_o <= '0';
   end if;
end if;    
end process;


end Behavioral;