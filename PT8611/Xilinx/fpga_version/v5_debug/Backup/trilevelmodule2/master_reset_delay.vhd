library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity master_reset_delay is
   port (
	   mreset_i             		: in std_logic;                        -- Master reset
	   clk_i                		: in std_logic;                        -- sample clock
	   tick_10ms_o                : out std_logic;
	   reset_delayed_o				: out std_logic
   );
end master_reset_delay;


architecture Behavioral of master_reset_delay is


signal tick_10ms_count           : std_logic_vector(20 downto 0);
signal delay_count               : std_logic_vector(7 downto 0);
signal tick_10ms                 : std_logic;

constant delay_val		         : integer :=255;


begin

-------------------------------------------------------------
-- Generates 10ms tick pulses
-------------------------------------------------------------
tick_10ms_generation : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
	   tick_10ms_count <= conv_std_logic_vector(1485000,21);
		tick_10ms <= '0';
	elsif clk_i'event and clk_i = '1' then
		if tick_10ms_count = conv_std_logic_vector(1,21) then
		   tick_10ms_count <= conv_std_logic_vector(1485000,21);
		   tick_10ms <= '1';
		else
         tick_10ms_count <= tick_10ms_count-1;
         tick_10ms <= '0';
   	end if;
	end if;
end process;
tick_10ms_o <= tick_10ms;


--------------------------------------------------------------
-- Delays the reset signal
--------------------------------------------------------------
reset_delaying : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		reset_delayed_o <= '0';
		delay_count <= conv_std_logic_vector(delay_val,8);
	elsif clk_i'event and clk_i = '1' then
	   if tick_10ms = '1' then
		   if delay_count = conv_std_logic_vector(0,8) then
		      reset_delayed_o <= '1';
	      end if;
	      delay_count <= delay_count-1;
	   end if;
	end if;
end process;


end Behavioral;