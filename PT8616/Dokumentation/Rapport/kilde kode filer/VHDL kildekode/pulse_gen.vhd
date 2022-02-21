-----------------------------------------------------------
--				 Pulse generator
-----------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity pulse_gen is
    Port ( clk_i : in  STD_LOGIC;
           rst_i : in  STD_LOGIC;
           per_hi_i : in  STD_LOGIC_VECTOR (19 downto 0);
           per_lo_i : in  STD_LOGIC_VECTOR (19 downto 0);
           per_offset_i : in  STD_LOGIC_VECTOR (7 downto 0);
           pulse_o : out  STD_LOGIC;
           start_state_i : in  STD_LOGIC);
end pulse_gen;

architecture Behavioral of pulse_gen is

signal pulse_state	:	std_logic;
signal count			:	std_logic_vector(19 downto 0);

begin

	count_period : process(clk_i, rst_i, start_state_i, per_offset_i )
	begin
		--reset
		if rst_i = '1' then
			--set start state, og count for reset
			pulse_state <= start_state_i;
			count <= "000000000000" & per_offset_i;
		
		elsif clk_i'event and clk_i='1' then
			if count = 0 then
				if pulse_state = '0' then
					pulse_state <= '1';
					count <= per_hi_i;
				else
					pulse_state <='0';
					count <= per_lo_i;
				end if;
			else
				count <= count - 1;
			end if;
		end if;
	end process count_period;
	
	pulse_o <= pulse_state;

end Behavioral;

