library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;

entity GPS_phase_loop is
    Port ( clk_i : in STD_LOGIC;
			  rst_i : in STD_LOGIC;
			  fix_i : in STD_LOGIC;					--1 = benyt midlet DAC værdi
			  OCXO_cv_value_i : in STD_LOGIC_VECTOR (15 downto 0);
			  fastslow_i : in STD_LOGIC;			--1 = hurtigt filter, 0 = langsomt filter
			  phase_diff_i : in  STD_LOGIC_VECTOR (27 downto 0);
           update_tick_i : in  STD_LOGIC;
           dac_word_o : out  STD_LOGIC_VECTOR (15 downto 0)
			  );
end GPS_phase_loop;

architecture Behavioral of GPS_phase_loop is

signal integrator : std_logic_vector (35 downto 0); --(32 bit heltal, 8 bit fraktion)
signal out_sum : std_logic_vector (27 downto 0);
signal integrator_limit : std_logic_vector (35 downto 0); --(32 bit heltal, 8 bit fraktion)
signal out_sum_limit : std_logic_vector (15 downto 0);

--signal avg_DAC_value : std_logic_vector (15 downto 0);

begin
	--opdatering og udregning af gennemsnits DAC værdi
--	update_DAC_output : process(clk_i)
--	begin
--		if clk_i'event and clk_i = '1' then
--			if rst_i = '1' then
--				avg_DAC_value <= OCXO_cv_value_i;
--			elsif update_tick_i = '1' then					
--				if fix_i = '0' then
--					--beregn gennemsnits DAC værdier (sidste 16)					
--					-- Y(n) = Y(n-1)-Y(n-1)/8 + X(n)/8
--					avg_DAC_value <= avg_DAC_value-("0000" & avg_DAC_value(15 downto 4)) + ("0000" & out_sum_limit(15 downto 4));
--				end if;
--			end if;
--		end if;
--	end process;
	
	with fix_i select
		dac_word_o <=	out_sum_limit when '0',
							OCXO_cv_value_i when others;

	
	GPS_phase_loop : process(clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if rst_i = '1' then
				integrator <= "000000000000" & OCXO_cv_value_i & "00000000";
				--integrator <= conv_std_logic_vector(4361984, 36);
			elsif update_tick_i = '1' then					
				--hvis hurtigt loop
				if fastslow_i = '1' then
					--hvis positiv fasefejl
					if phase_diff_i(27) = '0' then
						integrator <= integrator_limit + (phase_diff_i & "00000000");
					--hvis negativ fasefejl
					else
						integrator <= integrator_limit + (phase_diff_i & "00000000");		
					end if;
					
				--langsomt loop
				else
					--hvis positiv fasefejl
					if phase_diff_i(27) = '0' then 
						integrator <= integrator_limit + ("00000000" & phase_diff_i);		
					--hvis negativ fasefejl
					else
						integrator <= integrator_limit + ("11111111" & phase_diff_i);
					end if;
				end if;
			end if;
		end if;
	end process;
	
	-- opdater output summen
	update_outsum : process (phase_diff_i, integrator_limit, fastslow_i)
	begin
		--hurtigt loop
		if fastslow_i = '1' then
			--hvis positiv fasefejl
			if phase_diff_i(27) = '0' then
				out_sum <= integrator_limit(35 downto 8) + (phase_diff_i(25 downto 0) & "00");
			--hvis negativ fasefejl
			else
				out_sum <= integrator_limit(35 downto 8) + (phase_diff_i(25 downto 0) & "00");
			end if;
			
		--langsomt loop
		else
			--hvis positiv fasefejl
			if phase_diff_i(27) = '0' then 
				out_sum <= integrator_limit(35 downto 8) + ("00" & phase_diff_i(27 downto 2));
			--hvis negativ fasefejl
			else
				out_sum <= integrator_limit(35 downto 8) + ("11" & phase_diff_i(27 downto 2));
			end if;
		end if;
	end process;
	
	-- limiter for out sum + integrator
	phase_loop_limiter : process(integrator, out_sum)
	begin		
		-- limiter for integrator
		-- hvis negativ (min)
		if integrator(35) = '1' then
			integrator_limit <= conv_std_logic_vector(0, 36); -- ikke under 0
			
		-- hvis positiv (max)
		elsif integrator(35) = '0' and integrator > conv_std_logic_vector(11072768, 36) then
			integrator_limit <= conv_std_logic_vector(11072768, 36); -- ikke over 43253*256
		
		else
			integrator_limit <= integrator;
		end if;
		
		-- limiter for output
		-- hvis negativ (min)
		if out_sum(27) = '1' then 
			out_sum_limit <= conv_std_logic_vector(0, 16); -- ikke under 0
			
		-- hvis positiv (max)
		elsif out_sum(27) = '0' and out_sum > conv_std_logic_vector(43253, 28) then
			out_sum_limit <= conv_std_logic_vector(43253, 16); -- ikke over 43253
			
		else 
			out_sum_limit <= out_sum(15 downto 0);
		end if;	
	end process;
	
end Behavioral;

