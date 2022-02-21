----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:25:39 01/18/2007 
-- Design Name: 
-- Module Name:    pulse_gen - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity pulse_gen is
    Port ( clk_i : in  STD_LOGIC;
           rst_i : in  STD_LOGIC;
           per_hi_i : in integer;
           per_lo_i : in integer;
           per_offset_i : in integer;
           pulse_o : out  STD_LOGIC);
end pulse_gen;

architecture Behavioral of pulse_gen is

signal pulse_state	:	std_logic;
signal count			:	integer;

begin
	count_period : process(clk_i)
	begin
		if clk_i'event and clk_i='1' then
			--reset
			if rst_i = '1' then
				--set start state, og count for reset
				pulse_state <= '0';
				count <= per_offset_i;
			else
				if count = 1 then
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
		end if;
	end process count_period;
	
	pulse_o <= pulse_state;

end Behavioral;

