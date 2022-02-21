-----------------------------------------------------------
--				 Clock kompensator
-----------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;

entity clock_compensate is
   port (
      rst_i				:	in	std_logic;
		clk_i				:	in std_logic;
		pps_i				:	in	std_logic;
		synth_pps_o		:	out std_logic;
		phase_diff_o 	:	out std_logic_vector(7 downto 0);
		faster_tick		:	out std_logic;
		slower_tick		:	out std_logic
   );
end clock_compensate;

architecture Behavioral of clock_compensate is
-- 1 pps syntese
signal sec_pulse				:	std_logic;
signal pps_count 				:	std_logic_vector (28 downto 0);
signal phase_count			:	std_logic_vector (28 downto 0);
signal pps_i_delayline		:	std_logic_vector (1 downto 0);
signal pps_phase_diff		:	std_logic_vector (7 downto 0);
signal shorter_pers			:	std_logic_vector (7 downto 0);
signal longer_pers			:	std_logic_vector (7 downto 0);
signal make_longer			:	std_logic;
signal make_shorter			:	std_logic;
signal pps_first_time_flag	:	std_logic;

begin
	--generer 1 Hz 50:50 puls i sync med 1PPS fra GPS
	pps_gen : process(clk_i, rst_i)
	begin
		if clk_i'event and clk_i='1' then
			if rst_i='1' then
				pps_count <= "11111111111111111111111111111";
				phase_count <= conv_std_logic_vector(74250000, 29);
				sec_pulse <= '1';
				pps_first_time_flag <= '1';
				pps_i_delayline <= "00";
				shorter_pers <= "00000000";
				longer_pers <= "00000000";
			else
				--generer 50:50 duty cycle 
				if pps_count = 0 and sec_pulse = '1' then
					phase_count <= conv_std_logic_vector(74250000, 29);
					pps_count <= conv_std_logic_vector(74249999, 29);
					sec_pulse <= '0';
				elsif pps_count = 0 and sec_pulse = '0' then
					pps_count <= conv_std_logic_vector(74249999, 29);
					sec_pulse <= '1';
				else
					phase_count <= phase_count-1;
					pps_count <= pps_count-1;
				end if;
			
				-- synkroniser med GPS 1PPS, efter stabilisering ved opstart
				pps_i_delayline(1) <= pps_i_delayline(0);
				pps_i_delayline(0) <= pps_i;
				
				if pps_i_delayline = "01" and pps_first_time_flag = '1' then
					pps_count <= conv_std_logic_vector(74249999, 29);
					pps_first_time_flag <= '0';
					
				-- mål faseforskydning på free-run pps og GPS PPS
				elsif pps_i_delayline = "01" and pps_first_time_flag = '0' then
					pps_phase_diff <= phase_count(7 downto 0);
					
					--positiv forskydning
					if phase_count > 0 then
						shorter_pers <= shorter_pers + 1;
						longer_pers <= "00000000";
						
						if shorter_pers > "00000011" then
							slower_tick <= '1';
							shorter_pers <= "00000000";
						end if;
						
					--negativ forskydning
					elsif phase_count < 0 then
						longer_pers <= longer_pers + 1;
						shorter_pers <= "00000000";
						
						if longer_pers > "00000011" then
							faster_tick <= '1';
							longer_pers <= "00000000";
						end if;
					--ingen forskydning
					else
						longer_pers <= "00000000";
						shorter_pers <= "00000000";
					end if;
				else
					faster_tick <= '0';
					slower_tick <= '0';
				end if;			
						
			end if; --reset = '1'
		end if; -- clk'event
	end process;
	
	synth_pps_o <= sec_pulse;
	phase_diff_o <= pps_phase_diff;

end Behavioral;

