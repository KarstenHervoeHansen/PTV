
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity ltc_biphase_gen is
    Port ( 	clk_i : in  STD_LOGIC;
				ltc_clock_tick_i : in  STD_LOGIC;
				ltc_bitcount_i : in STD_LOGIC_VECTOR(7 downto 0);
				ltc_frame_i : in  STD_LOGIC_VECTOR (79 downto 0);
				biphase_o : out  STD_LOGIC);
end ltc_biphase_gen;

architecture Behavioral of ltc_biphase_gen is

-- intern komponent bifase signal
signal biphase_code		: STD_LOGIC:='1';

begin
	biphase_generator : process (clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if ltc_clock_tick_i = '1' then
				-- generer biphase bitrække
				if ltc_bitcount_i(0) = '0' then
					biphase_code <= not biphase_code;
				else
					if ltc_frame_i(conv_integer(79-ltc_bitcount_i(7 downto 1))) = '1' then
						biphase_code <= not biphase_code;
					end if;
				end if;
			end if;
		end if;
	end process;
	
	--output biphase code
	biphase_o <= biphase_code;
end Behavioral;