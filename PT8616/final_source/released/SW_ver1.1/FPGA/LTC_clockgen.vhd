
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity LTC_clockgen is
    Port ( clk_i : in  STD_LOGIC;
           sync_i : in  STD_LOGIC;
           format_i : in  STD_LOGIC_VECTOR (1 downto 0);
           ltc_clock_tick_o : out  STD_LOGIC;
			  ltc_frame_tick_o : out STD_LOGIC;
			  ltc_sync_o : out STD_LOGIC;
			  ltc_bitcount_o : out STD_LOGIC_VECTOR(7 downto 0));
end LTC_clockgen;

architecture Behavioral of LTC_clockgen is

-- counter til at generere clockticks
signal period_count	: integer range 0 to 40000; --downclock tæller

--constant per_24_FPS	: integer := 5;		--24 FPS 38672
--constant per_25_FPS	: integer := 5;		--25 FPS 37125
--constant per_2997_FPS	: integer := 5;	--30/1,001 FPS 30969
--constant per_30_FPS : integer := 5;			-- 30 FPS 30937

constant per_24_FPS	: integer := 38672;		--24 FPS 38672
constant per_25_FPS	: integer := 37125;		--25 FPS 37125
constant per_2997_FPS	: integer := 30969;	--30/1,001 FPS 30969
constant per_30_FPS : integer := 30938; 		-- 30 FPS 30938

--interne signaler
signal ltc_bitcount : std_logic_vector(7 downto 0);

begin
	bit_clock_generator : process(clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			--hvis resync
			if sync_i = '1' then
				ltc_clock_tick_o <= '1';
				ltc_frame_tick_o <= '1';
				ltc_sync_o <= '1';
				ltc_bitcount <= "00000000";
				
				case format_i is
					when "00" =>
						period_count <= per_24_FPS;
					when "01" =>
						period_count <= per_25_FPS;
					when "10" =>
						period_count <= per_2997_FPS;
					when others =>
						period_count <= per_30_FPS;
				end case;
				
			--hvis en hel bit-periode
			elsif period_count = 1 then
				ltc_clock_tick_o <= '1';
				ltc_sync_o <= '0';
				
				if ltc_bitcount = conv_std_logic_vector(159, 8) then
					ltc_frame_tick_o <= '1';
					ltc_bitcount <= "00000000";
				else
					ltc_bitcount <= ltc_bitcount + 1;
				end if;
				
				--reload periode-counter
				case format_i is
					when "00" =>
						period_count <= per_24_FPS;
					when "01" =>
						period_count <= per_25_FPS;
					when "10" =>
						period_count <= per_2997_FPS;
					when others =>
						period_count <= per_30_FPS;
				end case;
				
			--ellers tæl på periode-counter
			else
				period_count <= period_count-1;
				ltc_clock_tick_o <= '0';
				ltc_frame_tick_o <= '0';
				ltc_sync_o <= '0';
			end if; 
		end if;
	end process;
		
	ltc_bitcount_o <= ltc_bitcount;
end Behavioral;

