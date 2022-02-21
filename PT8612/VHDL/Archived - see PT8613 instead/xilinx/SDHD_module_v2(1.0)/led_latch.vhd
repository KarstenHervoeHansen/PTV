library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity led_latch is
   port (
	   led_i          				: in std_logic;
	   clk_i                		: in std_logic;                        -- sample clock
	   tick_10ms_i                : in std_logic;
	   led_o          				: out std_logic
   );
end led_latch;


architecture Behavioral of led_latch is


signal tick_10ms_count           : std_logic;


begin

--------------------------------------------------------------
-- Latches the led signal:
--------------------------------------------------------------
led_latching : process (led_i, clk_i) begin
	if led_i = '1' then
		led_o <= '1';
		tick_10ms_count <= '1';
	elsif clk_i'event and clk_i = '1' then
	   if tick_10ms_i = '1' then
	      if tick_10ms_count = '0' then
	         led_o <= '0';
	      else
	         tick_10ms_count <= '0';
	      end if;
	   end if;
	end if;
end process;


end Behavioral;