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
	   mreset_i      	      : in std_logic;                     -- Master reset
	   frame_start_i        : in std_logic;                     -- tick at frame start
	   clk_i         	      : in std_logic;                     -- sample clock
	   spl_div_i            : in std_logic_vector(9 downto 0);  -- total samples pr line divided by 10 or 11
      div10_11_i           : in std_logic;                     -- divider f148 med 10 hvis lav, 11 hvis høj
	   start_line_pulse_o   : out std_logic;
	   mid_line_pulse_o     : out std_logic;
	   line_number_o        : out std_logic_vector(10 downto 0)
   );
end frame_counter;


architecture Behavioral of frame_counter is

signal count1                 : std_logic_vector(3 downto 0);
signal count_val1             : std_logic_vector(3 downto 0);
signal load1                  : std_logic;
signal count1_is_one          : std_logic;
signal count2_is_one_delayed1 : std_logic;
signal count2                 : std_logic_vector(8 downto 0);
signal count_val2             : std_logic_vector(8 downto 0);
signal load2                  : std_logic;
signal en2                    : std_logic;
signal count3                 : std_logic_vector(11 downto 0);
signal count_val3             : std_logic_vector(11 downto 0);
signal load3                  : std_logic;
signal en3                    : std_logic;
signal half_line_pulse        : std_logic;
signal line_mid_tog           : std_logic;
signal frame_start_delayed1   : std_logic;


begin


--------------------------------------------------------------
-- Counter 1 counts to 10 or 11:
--------------------------------------------------------------
count_val1 <= "101" & div10_11_i;
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
count_val2 <= spl_div_i(9 downto 1);
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
-- Generate half line pulse
--------------------------------------------------------------
half_line_pulsing : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                        
   half_line_pulse <= '0';
elsif clk_i'event and clk_i = '1' then
   if (count1_is_one = '1' and count2_is_one_delayed1 = '1') or frame_start_i = '1' then
      half_line_pulse <= '1';
   else
      half_line_pulse <= '0';
   end if;
end if;    
end process;


--------------------------------------------------------------
-- half line counting (and line counting)
--------------------------------------------------------------
frame_start_delaying : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                        
   frame_start_delayed1 <= '0';
elsif clk_i'event and clk_i = '1' then
   frame_start_delayed1 <= frame_start_i;
end if;
end process;

count_val3 <= conv_std_logic_vector(2,12);
en3 <= half_line_pulse;
load3 <= frame_start_delayed1;

half_line_counting : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                        
   count3 <= count_val3;
elsif clk_i'event and clk_i = '1' then
   if load3 = '1' then
      count3 <= count_val3;
   elsif en3 = '1' then
      count3 <= count3 + 1;
   end if;
end if;    
end process;


line_number_o <= count3(11 downto 1);
line_mid_tog <= count3(0);


--------------------------------------------------------------
-- Line start pulse
--------------------------------------------------------------
line_start_pulsing : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                        
   start_line_pulse_o <= '0';
elsif clk_i'event and clk_i = '1' then
   if half_line_pulse = '1' and line_mid_tog = '0' then
      start_line_pulse_o <= '1';
   else
      start_line_pulse_o <= '0';
   end if;
end if;    
end process;


--------------------------------------------------------------
-- Line mid pulse
--------------------------------------------------------------
line_mid_pulsing : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                        
   mid_line_pulse_o <= '0';
elsif clk_i'event and clk_i = '1' then
   if half_line_pulse = '1' and line_mid_tog = '1' then
      mid_line_pulse_o <= '1';
   else
      mid_line_pulse_o <= '0';
   end if;
end if;    
end process;


end Behavioral;