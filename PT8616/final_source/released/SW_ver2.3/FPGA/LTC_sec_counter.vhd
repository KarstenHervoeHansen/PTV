----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:10:48 12/21/2007 
-- Design Name: 
-- Module Name:    LTC_sec_counter - Behavioral 
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

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity LTC_sec_counter is
    Port ( clk_i : in  STD_LOGIC;
           sec_tick_i : in  STD_LOGIC);
end LTC_sec_counter;

architecture Behavioral of LTC_sec_counter is

begin
-- sekund tæller
	sec_counter : process (clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if ltc_sync_i = '1' then	-- resettes for hvert sekund
				hour_tens <= hours_i(5 downto 4);
				hour_units <= hours_i(3 downto 0);
				min_tens <= mins_i(6 downto 4);
				min_units <= mins_i(3 downto 0);
				sec_tens <= secs_i(6 downto 4);
				sec_units <= secs_i(3 downto 0);
				
			elsif count_sec = '1' then						-- resettes for hvert sekund
				--hvis talt til 9 sekunder, inkrementer sek 10'ere
				if sec_units = "1001" then
					sec_units <= "0000";
					--hvis talt til 59 sekunder, inkrementer min
					if sec_tens = "101" then
						sec_tens <= "000";
						--hvis talt op til 9 minutter, inkrementer min 10'ere
						if min_units = "1001" then
							min_units <= "0000";
							--hvis talt op til 59 minutter, inkrementer timer 1'ere
							if min_tens = "101" then
								min_tens <= "000";

								--hvis talt op til 9 timer (00-19), inkrementer timer 10'ere
								if hour_units = "1001" and hour_tens /="10" then
									hour_tens <= hour_tens + 1;
									hour_units <= "0000";
								--hvis talt til 23, wrap rundt
								elsif hour_units = "0011" and hour_tens ="10" then
									hour_tens <= "00";
									hour_units <= "0000";
								else
									hour_units <= hour_units + 1;
								end if;
							--hvis under 59 min
							else
								min_tens <= min_tens + 1;
							end if;
						--hvis under 9 min
						else
							min_units <= min_units + 1;
						end if;
					--hvis under 59 sek
					else
						sec_tens <= sec_tens + 1;
					end if;
				--hvis under 9 sek
				else				
					sec_units <= sec_units + 1;
				end if;
			end if;
		end if;
	end process;
	
	update_out : process(clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if ltc_frame_tick_i = '1' and ltc_sync_i = '1' then
				hour_tens_o <= hours_i(4) & hours_i(5);
				hour_units_o <= hours_i(0) & hours_i(1) & hours_i(2) & hours_i(3);
				min_tens_o <=  mins_i(4) & mins_i(5) & mins_i(6);
				min_units_o <= mins_i(0) & mins_i(1) & mins_i(2) & mins_i(3);
				sec_tens_o <= secs_i(4) & secs_i(5) & secs_i(6);
				sec_units_o <= secs_i(0) & secs_i(1) & secs_i(2) & secs_i(3);
			elsif update_output = '1' then
				hour_tens_o <= hour_tens(0) & hour_tens(1);
				hour_units_o <= hour_units(0) & hour_units(1) & hour_units(2) & hour_units(3);
				min_tens_o <= min_tens(0) & min_tens(1) & min_tens(2);
				min_units_o <= min_units(0) & min_units(1) & min_units(2) & min_units(3);
				sec_tens_o <= sec_tens(0) & sec_tens(1) & sec_tens(2);
				sec_units_o <= sec_units(0) & sec_units(1) & sec_units(2) & sec_units(3);
			end if;
		end if;
	end process;
end Behavioral;

