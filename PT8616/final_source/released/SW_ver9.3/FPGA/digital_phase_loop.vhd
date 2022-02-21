library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;

entity digital_phase_loop is
    Port ( --rst_i : in STD_LOGIC;
			  clk_i : in STD_LOGIC;
			  phase_diff_i : in  STD_LOGIC_VECTOR (12 downto 0);
			  update_tick_i : in  STD_LOGIC;
			  dac_word_o : out  STD_LOGIC_VECTOR (15 downto 0)
			  );
end digital_phase_loop;

architecture Behavioral of digital_phase_loop is

signal integrator : std_logic_vector (31 downto 0):=(others => '0'); 
signal out_sum : std_logic_vector (17 downto 0):=(others => '0');

begin

	-- output digital styreværdi til DAC
	dac_word_o <= out_sum(17 downto 2);
		
	-- opdater udsum
	update_out_sum : process(clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if update_tick_i = '1' then
				--proportional del
				if phase_diff_i(11) = '0' then --hvis positiv fasefejl
					out_sum <= integrator(31 downto 14) + ( "00" & phase_diff_i(12 downto 0) & "000" );	--beta 32
				else 									--hvis negativ fasefejl
					out_sum <= integrator(31 downto 14) + ( "11" & phase_diff_i(12 downto 0) & "000");
				end if;
			end if;
		end if;
	end process;

	-- integrator
	integrator_update : process(clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if update_tick_i = '1' then								
				--integrator (by only using 11 -> 2 we cut the quatization error giving us 2 clk uncertainty, but less jitter)
				if phase_diff_i(11) = '0' then --hvis positiv fasefejl
					integrator <= integrator + ("0000000000000000000" & phase_diff_i(12 downto 0));
				else 									--hvis negativ fasefejl
					integrator <= integrator + ("1111111111111111111" & phase_diff_i(12 downto 0));
				end if;
			end if;
		end if;
	end process;

end Behavioral;

