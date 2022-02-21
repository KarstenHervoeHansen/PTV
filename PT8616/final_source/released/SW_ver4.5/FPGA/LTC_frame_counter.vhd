----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:56:33 11/07/2007 
-- Design Name: 
-- Module Name:    LTC_frame_counter - Behavioral 
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

entity LTC_frame_counter is
    Port ( 	
				-- inputs
				clk_i : in  STD_LOGIC;
				ltc_frame_tick_i : in STD_LOGIC;
				ltc_sync_i : in STD_LOGIC;
				dropframe_i : in STD_LOGIC;
				format_i : in STD_LOGIC_VECTOR (1 downto 0);
				hours_i : in STD_LOGIC_VECTOR(7 downto 0);
				mins_i : in STD_LOGIC_VECTOR(7 downto 0);
				secs_i : in STD_LOGIC_VECTOR(7 downto 0);
				frames_i : in STD_LOGIC_VECTOR(7 downto 0);
				--outputs
				hour_tens_o : out STD_LOGIC_VECTOR(1 downto 0);
				hour_units_o : out STD_LOGIC_VECTOR(3 downto 0);
				min_tens_o : out STD_LOGIC_VECTOR(2 downto 0);
				min_units_o : out STD_LOGIC_VECTOR(3 downto 0);
				sec_tens_o : out STD_LOGIC_VECTOR(2 downto 0);
				sec_units_o : out STD_LOGIC_VECTOR(3 downto 0);
				frame_tens_o : out STD_LOGIC_VECTOR(1 downto 0);
				frame_units_o : out STD_LOGIC_VECTOR(3 downto 0)
	 );
				
end LTC_frame_counter;

architecture Behavioral of LTC_frame_counter is

signal hour_tens : STD_LOGIC_VECTOR(1 downto 0);
signal hour_units : STD_LOGIC_VECTOR(3 downto 0);
signal min_tens : STD_LOGIC_VECTOR(2 downto 0);
signal min_units : STD_LOGIC_VECTOR(3 downto 0);
signal sec_tens : STD_LOGIC_VECTOR(2 downto 0);
signal sec_units : STD_LOGIC_VECTOR(3 downto 0);
signal frame_tens : STD_LOGIC_VECTOR(1 downto 0);
signal frame_units : STD_LOGIC_VECTOR(3 downto 0);

signal count_sec : STD_LOGIC;
--signal update_out_tick : STD_LOGIC;

begin
	--tæl frames
	frame_counter : process (clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if ltc_frame_tick_i = '1' and ltc_sync_i = '1' then	-- resettes for hvert sekund
				frame_tens <= frames_i(5 downto 4);
				frame_units <= frames_i(3 downto 0);
				
			elsif ltc_frame_tick_i = '1' then
				--hvis 24 fps
				if format_i = "00" then
					--hvis talt til 9 frames, tæl én på 10'ere og reset 1'ere
					if frame_units = "1001" then
						frame_units <= "0000";
						frame_tens <= frame_tens + 1;
					--hvis 23. frame, reset, og tæl sekund
					elsif frame_units = "0011" and frame_tens = "10" then
						frame_units <= "0000";
						frame_tens <= "00";
					else
						frame_units <= frame_units + 1;
					end if;
					
				--hvis 25 fps
				elsif format_i = "01" then
					--hvis talt til 9 frames, tæl én på 10'ere og reset 1'ere
					if frame_units = "1001" then
						frame_units <= "0000";
						frame_tens <= frame_tens + 1;
					--hvis 24. frame, reset, og tæl sekund
					elsif frame_units = "0100" and frame_tens = "10" then
						frame_units <= "0000";
						frame_tens <= "00";
					else
						frame_units <= frame_units + 1;
					end if;
					
				--hvis 29.97 non-df fps
				elsif format_i = "10" and dropframe_i = '0' then
					--hvis talt til 9 frames, tæl én på 10'ere og reset 1'ere
					if frame_units = "1001" then
						frame_units <= "0000";
						frame_tens <= frame_tens + 1;
						
						if frame_tens = "10" then
							frame_tens <= "00";
						end if;
					else
						frame_units <= frame_units + 1;
					end if;
					
				--hvis 29.97 df fps
				elsif format_i = "10" and dropframe_i = '1' then
					--hvis talt til 9 frames, tæl én på 10'ere og reset 1'ere
					if frame_units = "1001" then
						if frame_tens = "10" and sec_units = "1001" and sec_tens = "101" and min_units /= "1001" then
							frame_units <= "0010";
							frame_tens <= "00";
						elsif frame_tens = "10" then
							frame_units <= "0000";
							frame_tens <= "00";
						else
							frame_units <= "0000";
							frame_tens <= frame_tens + 1;
						end if;
					else
						frame_units <= frame_units + 1;
					end if;
					
				--hvis 30 fps
				else
					--hvis talt til 9 frames, tæl én på 10'ere og reset 1'ere
					if frame_units = "1001" then
						frame_units <= "0000";
						frame_tens <= frame_tens + 1;
						
						if frame_tens = "10" then
							frame_tens <= "00";
						end if;
					else
						frame_units <= frame_units + 1;
					end if;
				end if;
			end if;
		end if;
	end process;

					
	sec_count_tick_gen : process (clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if ltc_frame_tick_i = '1' then
				-- lav tick der tæller ét sekund for hver framerate
				if format_i = "00" and frame_units = "0011" and frame_tens = "10" then
					count_sec <= '1';
				elsif format_i = "01" and frame_units = "0100" and frame_tens = "10" then
					count_sec <= '1';
				elsif format_i = "10" and frame_units = "1001" and frame_tens = "10" then
					count_sec <= '1';
				elsif format_i = "11" and frame_units = "1001" and frame_tens = "10" then
					count_sec <= '1';
				end if;
			else
				count_sec <= '0';
			end if;
		end if;
	end process;
								
	sec_counter : process (clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if ltc_frame_tick_i = '1' and ltc_sync_i = '1' then	-- resettes for hvert sekund
				hour_tens <= hours_i(5 downto 4);
				hour_units <= hours_i(3 downto 0);
				min_tens <= mins_i(6 downto 4);
				min_units <= mins_i(3 downto 0);
				sec_tens <= secs_i(6 downto 4);
				sec_units <= secs_i(3 downto 0);
				
			elsif count_sec = '1' then
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
			--if update_out_tick = '1' then
			if ltc_frame_tick_i = '1' then
				hour_tens_o <= hour_tens(0) & hour_tens(1);
				hour_units_o <= hour_units(0) & hour_units(1) & hour_units(2) & hour_units(3);
				min_tens_o <= min_tens(0) & min_tens(1) & min_tens(2);
				min_units_o <= min_units(0) & min_units(1) & min_units(2) & min_units(3);
				sec_tens_o <= sec_tens(0) & sec_tens(1) & sec_tens(2);
				sec_units_o <= sec_units(0) & sec_units(1) & sec_units(2) & sec_units(3);
				frame_tens_o <= frame_tens(0) & frame_tens(1);
				frame_units_o <= frame_units(0) & frame_units(1) & frame_units(2) & frame_units(3);
			end if;
		end if;
	end process;
				
end Behavioral;

