library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity frame_counter is
   port (
	   mreset_i      	: in std_logic;                     -- Master reset
	   frame_start_i  : in std_logic;                     -- tick at frame start
	   clk_i         	: in std_logic;                     -- sample clock
	   spl_div_i      : in std_logic_vector(9 downto 0);  -- total samples pr line divided by 10 or 11
      div10_11_i     : in std_logic;                     -- divider f148 med 10 hvis lav, 11 hvis høj
	   lpf_i          : in std_logic_vector(10 downto 0); -- total lines per frame
	   mid_line_o     : out std_logic
   );
end frame_counter;


architecture Behavioral of frame_counter is

signal count1        : std_logic_vector(3 downto 0);
signal count_val1    : std_logic_vector(3 downto 0);
signal count2        : std_logic_vector(8 downto 0);
signal count_val2    : std_logic_vector(8 downto 0);

begin


--------------------------------------------------------------
-- Counter 1 counts to 10 or 11:
--------------------------------------------------------------
count_val1 = "101" & div10_11_i;
load1 <= frame_start_i or count1_is_one;

counting_1 : process(mreset_i, clk_i)
begin
if mreset_i = '0' then
   count1 <= count_val1;
elsif clk_i'event and clk_i = '1' then
   if load1 = '1' then
      count1 <= count_val1;
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
   if count1 = conv_std_logic_vector(2,4) then
      count1_is_one <= '1';
   else
      count1_is_one <= '0';
   end if;
end if;
end process;


--------------------------------------------------------------
-- Counter 2 counts to a half line
--------------------------------------------------------------
count_val2 = spl_div_i(9 downto 1);
en2 <= count1_is_one;
load2 <= frame_start_i or (count2_is_one_delayed1 and count1_is_one);


counting_2 : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                          
   count2 <= count_val2;
elsif clk_i'event and clk_i = '1' then
   if load2 = '1' then
      count2 <= count_val2;
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
   if count2 = conv_std_logic_vector(1,9) then
      count2_is_one_delayed1 <= '1';
   else
      count2_is_one_delayed1 <= '0';
   end if;
end if;
end process;


--------------------------------------------------------------
-- Generate mid line pulse
--------------------------------------------------------------
mid_line_pulsing : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                        
   mid_line_o <= '0';
elsif clk_i'event and clk_i = '1' then
   if count1_is_one = '1' and count2_is_one_delayed1 = '1' then
      mid_line_o <= '1';
   else
      mid_line_o <= '0';
   end if;
end if;    
end process;


end Behavioral;