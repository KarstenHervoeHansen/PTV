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
      count_val1     : integer;            -- must be >= 3
      count_val2     : integer;            -- must be >= 3
      tick_val1      : integer := 2;
      tick_val2      : integer := 1;
      bits1          : integer;
      bits2          : integer
   );                
   Port(             
      mreset_i       : in std_logic;
      clk_i          : in std_logic;
      sync_reset_i   : in std_logic;
      sync_o         : out std_logic;
      sync_offset_o  : out std_logic
   );
end period_dual_count;

architecture Behavioral of period_dual_count is

signal count1                 : std_logic_vector(bits1-1 downto 0);
signal count2                 : std_logic_vector(bits2-1 downto 0);
signal count1_is_one          : std_logic;
signal count2_is_one_delayed1 : std_logic;
signal load1                  : std_logic;
signal load2                  : std_logic;
signal en2                    : std_logic;
signal count1_tick             : std_logic;
signal count2_tick             : std_logic;


begin


--------------------------------------------------------------
-- Counter 1
--------------------------------------------------------------
load1 <= sync_reset_i or count1_is_one;

counting_1 : process(mreset_i, clk_i)
begin
if mreset_i = '0' then
   count1 <= conv_std_logic_vector(count_val1,bits1);
elsif clk_i'event and clk_i = '1' then
   if load1 = '1' then
      count1 <= conv_std_logic_vector(count_val1,bits1);
   else
      count1 <= count1 - 1;
   end if;
end if;
end process;

count1_wraping : process(mreset_i, clk_i)
begin
if mreset_i = '0' then
   count1_is_one <= '0';
elsif clk_i'event and clk_i = '1' then
   if count1 = conv_std_logic_vector(2,bits1) then
      count1_is_one <= '1';
   else
      count1_is_one <= '0';
   end if;
end if;
end process;


--------------------------------------------------------------
-- Counter 2
--------------------------------------------------------------
en2 <= count1_is_one;
load2 <= sync_reset_i or (count2_is_one_delayed1 and count1_is_one);


counting_2 : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                          
   count2 <= conv_std_logic_vector(count_val2,bits2);
elsif clk_i'event and clk_i = '1' then
   if load2 = '1' then
      count2 <= conv_std_logic_vector(count_val2,bits2);
   elsif en2 ='1' then
      count2 <= count2 - 1;
   end if;
end if;    
end process;


count2_wraping : process(mreset_i, clk_i)
begin
if mreset_i = '0' then
   count2_is_one_delayed1 <= '0';
elsif clk_i'event and clk_i = '1' then
   if count2 = conv_std_logic_vector(1,bits2) then
      count2_is_one_delayed1 <= '1';
   else
      count2_is_one_delayed1 <= '0';
   end if;
end if;
end process;




--------------------------------------------------------------
-- Compare count1 and tick_val1:
--------------------------------------------------------------
tick_val1_comparing : process(mreset_i, clk_i)
begin
if mreset_i = '0' then
   count1_tick <= '0';
elsif clk_i'event and clk_i = '1' then
   if count1 = conv_std_logic_vector(tick_val1,bits1) then
      count1_tick <= '1';
   else
      count1_tick <= '0';
   end if;
end if;
end process;


--------------------------------------------------------------
-- Compare count2 and tick_val2:
--------------------------------------------------------------
tick_val2_comparing : process(mreset_i, clk_i)
begin
if mreset_i = '0' then
   count2_tick <= '0';
elsif clk_i'event and clk_i = '1' then
   if count2 = conv_std_logic_vector(tick_val2,bits2) then
      count2_tick <= '1';
   else
      count2_tick <= '0';
   end if;
end if;
end process;


--------------------------------------------------------------
-- Generate sync_o pulse
--------------------------------------------------------------
sync_offset_generation : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                        
   sync_offset_o <= '0';
elsif clk_i'event and clk_i = '1' then
   if count1_tick = '1' and count2_tick = '1' then
      sync_offset_o <= '1';
   else
      sync_offset_o <= '0';
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
   if count1_is_one = '1' and count2_is_one_delayed1 = '1' then
      sync_o <= '1';
   else
      sync_o <= '0';
   end if;
end if;    
end process;


end Behavioral;