-- Navn			: BB1_PLAY_LINE7
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Setup line7-adr and Writes data from the line7-reg. to the dac
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;



ENTITY BB1_PLAY_LINE7 IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_start_line7			:	IN	BIT;

		pga1_bb1_line7_reg				:	OUT	STD_LOGIC_VECTOR(8 DOWNTO 0);
--		pga1_bb1_line7_reg				:	OUT	STD_LOGIC_VECTOR(4 DOWNTO 0);
		pga1_bb1_load_line7			:	OUT	BIT
		); 
END BB1_PLAY_LINE7;

ARCHITECTURE LINE7_DATA2DAC OF BB1_PLAY_LINE7 IS
	TYPE STATE_TYPE IS (s0_up_30,s1_up_100,s2_up_225,s3_up_350,s4_up_475,
s5_up_600,s6_up_670,s7_up_700,s8_down_670,s9_down_600,s10_down_475,s11_down_350,s12_down_225,
s13_down_100,s14_down_30);
	SIGNAL state: STATE_TYPE;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
-- NB!! all IRE values shall be veryfied.

				WHEN s0_up_30 =>
					IF pga1_bb1_start_line7 = '1' THEN
						pga1_bb1_line7_reg(8 DOWNTO 0) <= "010111011"; -- 30mV
--						pga1_bb1_line7_reg(4 DOWNTO 0) <= "01011"; -- 30mV
						state <= s1_up_100;
						pga1_bb1_load_line7 <= '1';
					ELSE
						pga1_bb1_load_line7 <= '0';
						state <= s0_up_30;
					END IF;

				WHEN s1_up_100 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "011011010"; -- 100mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "01101"; -- 100mV
					state <= s2_up_225;

				WHEN s2_up_225 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "100001101"; -- 225mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "10000"; -- 225mV
					state <= s3_up_350;

				WHEN s3_up_350 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "100111111"; -- 350mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "10011"; -- 350mV
					state <= s4_up_475;

				WHEN s4_up_475 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "101110010"; -- 475mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "10111"; -- 475mV
					state <= s5_up_600;

				WHEN s5_up_600 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "110100100"; -- 600mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "11010"; -- 600mV
					state <= s6_up_670;

				WHEN s6_up_670 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "111000100"; -- 670mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "11100"; -- 670mV
					state <= s7_up_700;

				WHEN s7_up_700 =>
					IF pga1_bb1_start_line7 = '0' THEN
						state <= s7_up_700;
					ELSE
						state <= s8_down_670;
					END IF;

					pga1_bb1_line7_reg(8 DOWNTO 0) <= "111010100"; -- 700mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "11101"; -- 700mV

				WHEN s8_down_670 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "111000100"; -- 670mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "11100"; -- 670mV
					state <= s9_down_600;

				WHEN s9_down_600 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "110100100"; -- 600mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "11010"; -- 600mV
					state <= s10_down_475;

				WHEN s10_down_475 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "101110010"; -- 475mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "10111"; -- 475mV
					state <= s11_down_350;

				WHEN s11_down_350 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "100111111"; -- 350mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "10011"; -- 350mV
					state <= s12_down_225;

				WHEN s12_down_225 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "100001101"; -- 225mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "10000"; -- 225mV
					state <= s13_down_100;

				WHEN s13_down_100 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "011011010"; -- 100mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "01101"; -- 100mV
					state <= s14_down_30;

				WHEN s14_down_30 =>
					pga1_bb1_line7_reg(8 DOWNTO 0) <= "010111011"; -- 30mV
--					pga1_bb1_line7_reg(4 DOWNTO 0) <= "01011"; -- 30mV
					pga1_bb1_load_line7 <= '0';
					state <= s0_up_30;

			END CASE;
		END IF;
	END PROCESS;

END LINE7_DATA2DAC;



