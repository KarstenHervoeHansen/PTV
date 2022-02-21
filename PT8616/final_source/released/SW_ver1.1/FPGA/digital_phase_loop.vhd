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

signal integrator : std_logic_vector (25 downto 0); --(17 bit heltal, 8 bit fraktion)
signal out_sum : std_logic_vector (17 downto 0);
signal integrator_limit : std_logic_vector (25 downto 0); --(17 bit heltal, 8 bit fraktion)
signal out_sum_limit : std_logic_vector (15 downto 0);

begin
	-- output digital styreværdi til DAC
	dac_word_o <= out_sum_limit;
				
	-- opdater udsum
	update_out_sum : process(phase_diff_i, integrator_limit)
	begin
		--proportional del
		if phase_diff_i(12) = '0' then --hvis positiv fasefejl
			out_sum <= integrator_limit(25 downto 8) + ('0' & phase_diff_i & "0000");	--beta 32
		else 									--hvis negativ fasefejl
			out_sum <= integrator_limit(25 downto 8) + ('1' & phase_diff_i & "0000");
		end if;
	end process;
	
	-- integrator
	integrator_update : process(clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if update_tick_i = '1' then								
				--integrator
				if phase_diff_i(12) = '0' then --hvis positiv fasefejl
					integrator <= integrator_limit + ("0000000000" & phase_diff_i & "000"); --alpha 16
				else 									--hvis negativ fasefejl
					integrator <= integrator_limit + ("1111111111" & phase_diff_i & "000");
				end if;
			end if;
		end if;
	end process;
	
	-- limiter for integrator og output
	limiter : process(out_sum, integrator)
	begin
--		if rst_i = '1' then
--			integrator_limit <= "0000000000001111111111111";
--			out_sum_limit <= "0000000011111111";
--		end if;
		
		-- limiter for integrator
		if integrator(25) = '1' then
			integrator_limit <= conv_std_logic_vector(0, 26); -- ikke under 0
		elsif integrator(25) = '0' and integrator > conv_std_logic_vector(11072768, 26) then
			integrator_limit <= conv_std_logic_vector(11072768, 26); -- ikke over 43253
		else 
			integrator_limit <= integrator;
		end if;
		
		-- limiter for output
		if out_sum(17) = '1' then -- hvis negativ
			out_sum_limit <= conv_std_logic_vector(0, 17); -- ikke under 0
		elsif out_sum(17) = '0' and out_sum > conv_std_logic_vector(43253, 18) then
			out_sum_limit <= conv_std_logic_vector(43253, 17); -- ikke over 43253
		else 
			out_sum_limit <= out_sum(15 downto 0);
		end if;	
	end process;
end Behavioral;

