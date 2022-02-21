-----------------------------------------------------------
--				 LTC generator
-----------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity LTC_generator is
    Port ( hours_i	:	in STD_LOGIC_VECTOR (7 downto 0);
			  mins_i	: 	in STD_LOGIC_VECTOR (7 downto 0);
			  secs_i	:	in STD_LOGIC_VECTOR (7 downto 0);
			  format_select_i	:	in STD_LOGIC_VECTOR (1 downto 0);
           rst_i : in  STD_LOGIC;
           clk_i : in  STD_LOGIC;	-- 148.5 MHz master clock
           ltc_code_o : out  STD_LOGIC);
end LTC_generator;

architecture Behavioral of LTC_generator is

--main signals
signal format_select	: STD_LOGIC_VECTOR(1 downto 0); --valg af 24, 25 og 30/1,001 FPS

--modulation clock signals
signal ltc_clock_tick	: STD_LOGIC;
signal period_count		: STD_LOGIC_VECTOR (15 downto 0); --downclock tæller
signal period_num_count : STD_LOGIC_VECTOR (3 downto 0); --tæller for lange/korte perioder
signal long_short_select: STD_LOGIC:='0'; --skifter mellem at tælle korte='0'/lange='1' perioder

--24 FPS
constant long_per_24_FPS	: STD_LOGIC_VECTOR (15 downto 0):="1001011100010000";
constant short_per_24_FPS	: STD_LOGIC_VECTOR (15 downto 0):="1001011100001111";
constant long_per_24_numof	: STD_LOGIC_VECTOR (3 downto 0):="0111";
constant short_per_24_numof: STD_LOGIC_VECTOR (3 downto 0):="0001";

--25 FPS
constant long_per_25_FPS	: STD_LOGIC_VECTOR (15 downto 0):="1001000100000101";
constant short_per_25_FPS	: STD_LOGIC_VECTOR (15 downto 0):="1001000100000101";
constant long_per_25_numof	: STD_LOGIC_VECTOR (3 downto 0):="0011";
constant short_per_25_numof: STD_LOGIC_VECTOR (3 downto 0):="0011";

--30/1,001 FPS
constant long_per_30_FPS	: STD_LOGIC_VECTOR (15 downto 0):="0111100011111111";
constant short_per_30_FPS	: STD_LOGIC_VECTOR (15 downto 0):="0111100011111000";
constant long_per_30_numof	: STD_LOGIC_VECTOR (3 downto 0):="0001";
constant short_per_30_numof: STD_LOGIC_VECTOR (3 downto 0):="1111";


--biphase modulated frame signals
signal bit_count			: STD_LOGIC_VECTOR (7 downto 0);
signal biphase_code		: STD_LOGIC;
signal ltc_frame 			: STD_LOGIC_VECTOR (79 downto 0);

--timing frame signal
signal hours_tens			: STD_LOGIC_VECTOR (1 downto 0);
signal hours_units		: STD_LOGIC_VECTOR (3 downto 0);
signal mins_tens			: STD_LOGIC_VECTOR (2 downto 0);
signal mins_units			: STD_LOGIC_VECTOR (3 downto 0);
signal secs_tens			: STD_LOGIC_VECTOR (2 downto 0);
signal secs_units			: STD_LOGIC_VECTOR (3 downto 0);
signal frame_tens			: STD_LOGIC_VECTOR (0 to 1);
signal frame_units		: STD_LOGIC_VECTOR (0 to 3);

constant sync_word		: STD_LOGIC_VECTOR (15 downto 0):="0011111111111101";
constant BG					: STD_LOGIC_VECTOR (3 downto 0):="0000";

signal down_clk : STD_LOGIC_VECTOR(3 downto 0);

begin
	format_select <= format_select_i;

	code_generator : process(clk_i, rst_i)
	begin
--------------------------------------------------------------------------------	
		--LTC reset
		if rst_i = '1' then
			bit_count <= conv_std_logic_vector(0, 8);
			hours_tens <= hours_i(4) & hours_i(5);
			hours_units <= hours_i(0) & hours_i(1) & hours_i(2) & hours_i(3);
			mins_tens <= mins_i(4) & mins_i(5) & mins_i(6);
			mins_units <= mins_i(0) & mins_i(1) & mins_i(2) & mins_i (3);
			secs_tens <= secs_i(4) & secs_i(5) & secs_i(6);
			secs_units <= secs_i(0) & secs_i(1) & secs_i(2) & secs_i(3);
			frame_tens <= "00";
			frame_units <= "0000";

			ltc_clock_tick <= '0';
			long_short_select <= '0';
			period_count <= "0000000000000001";
			period_num_count <= "0001";
			
--------------------------------------------------------------------------------			
		--LTC clock gen
		elsif clk_i'event and clk_i='1' then
			period_count <= period_count-1;
			
			case format_select is
				--generate 24 FPS clock
				when "00" =>
					if period_count = 1 then
						ltc_clock_tick <= '1';
						period_num_count <= period_num_count-1;
						
						if period_num_count = 1 and long_short_select = '1' then
							long_short_select <= '0';
							period_num_count <= long_per_24_numof;
						elsif period_num_count = 1 and long_short_select = '0' then
							long_short_select <= '1';
							period_num_count <= short_per_24_numof;
						end if;
						
						if long_short_select = '0' then
							period_count <= long_per_24_FPS;
						else
							period_count <= short_per_24_FPS;
						end if;
					else
						ltc_clock_tick <= '0';
					end if;
				--generate 25 FPS clock
				when "01" =>
					if period_count = 1 then
						ltc_clock_tick <= '1';
						period_num_count <= period_num_count-1;
						
						if period_num_count = 1 and long_short_select = '1' then
							long_short_select <= '0';
							period_num_count <= long_per_25_numof;
						elsif period_num_count = 1 and long_short_select = '0' then
							long_short_select <= '1';
							period_num_count <= short_per_25_numof;
						end if;
						
						if long_short_select = '0' then
							period_count <= long_per_25_FPS;
						else
							period_count <= short_per_25_FPS;
						end if;
					else
						ltc_clock_tick <= '0';
					end if;
				--generate 30 FPS clock
				when "10" =>
					if period_count = 1 then
						ltc_clock_tick <= '1';
						period_num_count <= period_num_count-1;
						
						if period_num_count = 1 and long_short_select = '1' then
							long_short_select <= '0';
							period_num_count <= long_per_30_numof;
						elsif period_num_count = 1 and long_short_select = '0' then
							long_short_select <= '1';
							period_num_count <= short_per_30_numof;
						end if;
						
						if long_short_select = '0' then
							period_count <= long_per_30_FPS;
						else
							period_count <= short_per_30_FPS;
						end if;
					else
						ltc_clock_tick <= '0';
					end if;
				when others =>
					null;
			end case;		
			
--------------------------------------------------------------------------------	
			if ltc_clock_tick = '1' then
				down_clk <= down_clk +1 ;
	
				bit_count <= bit_count-1;
				--for hver frame
				if bit_count = 0 then
					bit_count <= conv_std_logic_vector(159, 8);
					hours_tens <= hours_i(4) & hours_i(5);
					hours_units <= hours_i(0) & hours_i(1) & hours_i(2) & hours_i(3);
					mins_tens <= mins_i(4) & mins_i(5) & mins_i(6);
					mins_units <= mins_i(0) & mins_i(1) & mins_i(2) & mins_i (3);
					secs_tens <= secs_i(4) & secs_i(5) & secs_i(6);
					secs_units <= secs_i(0) & secs_i(1) & secs_i(2) & secs_i(3);
				
					--opdater 80 bits i LTC frame
					ltc_frame <= frame_units & BG & frame_tens & "00" & BG & secs_units & BG & secs_tens & "0" & BG & mins_units & BG & mins_tens & "0" & BG & hours_units & BG & hours_tens & "00" & BG & sync_word;
				
					if down_clk = "1111" then
						--tæl frames
						frame_units <= frame_units + 1;
						
						if frame_units = "1001" then
							frame_tens <= frame_tens + 1;
							frame_units <= "0000";
						end if;
					end if;
					
					--reset frame tæller
					case format_select is
						--24 FPS
						when "00" =>
							if frame_tens = "10" and frame_units = "0011" then
								frame_tens <= "00";
								frame_units <= "0000";
							end if;
						--25 FPS
						when "01" =>
							if frame_tens = "01" and frame_units = "0010" then
								frame_tens <= "00";
								frame_units <= "0000";
							end if;
						--30 FPS
						when "10" =>
							if frame_tens = "01" and frame_units = "1001" then
								frame_tens <= "00";
								frame_units <= "0000";
							end if;
						--others = 25 FPS
						when others =>
							if frame_tens = "01" and frame_units = "0010" then
								frame_tens <= "00";
								frame_units <= "0000";
							end if;
					end case;
				end if;
				
				--for hver bit
				if bit_count(0) = '0' then
					biphase_code <= not biphase_code;
					
				elsif bit_count(0)='1' and ltc_frame(conv_integer(bit_count(7 downto 1)))='1' then
					biphase_code <= not biphase_code;
				end if;
			end if;
		end if; -- ltc_clock_tick = '1'
	end process code_generator;
	
	ltc_code_o <= biphase_code;
end Behavioral;

