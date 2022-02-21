-- Navn			: BB1_PLAY_PEDISTAL
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Setup pedistal-adr and Writes data from the pedistal-reg. to the dac
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;



ENTITY BB1_PLAY_PEDISTAL IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_start_pup				:	IN	BIT;
		pga1_bb1_start_pdown				:	IN	BIT;

		pga1_bb1_pedistal_reg			:	OUT	STD_LOGIC_VECTOR(3 DOWNTO 0);
		pga1_bb1_load_pedistal			:	OUT	BIT
		); 
END BB1_PLAY_PEDISTAL;

ARCHITECTURE PEDISTAL_DATA2DAC OF BB1_PLAY_PEDISTAL IS
	TYPE STATE_TYPE IS (s0_up_95,s1_up3_3,s2_up6_1,s3_up7_5,s4_down6_1,
s5_down3_3,s6_down_95,s7_down0);
	SIGNAL state: STATE_TYPE;
	CONSTANT L7_1 : STD_LOGIC_VECTOR(7 DOWNTO 0) := "00000010";

BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
-- NB!! all IRE values shall be veryfied.

				WHEN s0_up_95 =>
					IF pga1_bb1_start_pup = '1' THEN
						pga1_bb1_pedistal_reg(3 DOWNTO 0) <= "0010"; -- 0,95 IRE
						state <= s1_up3_3;
						pga1_bb1_load_pedistal <= '1';
					ELSE
						pga1_bb1_load_pedistal <= '0';
						state <= s0_up_95;
					END IF;

				WHEN s1_up3_3 =>
					pga1_bb1_pedistal_reg(3 DOWNTO 0) <= "0111"; -- 3,3 IRE
					state <= s2_up6_1;

				WHEN s2_up6_1 =>
					pga1_bb1_pedistal_reg(3 DOWNTO 0) <= "1100"; -- 6,1 IRE
					state <= s3_up7_5;

				WHEN s3_up7_5 =>
					IF pga1_bb1_start_pdown = '1' THEN
						state <= s4_down6_1;
					ELSE
						state <= s3_up7_5;
					END IF;

					pga1_bb1_pedistal_reg(3 DOWNTO 0) <= "1111"; -- 7,5 IRE
					
				WHEN s4_down6_1 =>
					pga1_bb1_pedistal_reg(3 DOWNTO 0) <= "1100"; -- 6,1 IRE
					state <= s5_down3_3;

				WHEN s5_down3_3 =>
					pga1_bb1_pedistal_reg(3 DOWNTO 0) <= "0111"; -- 3,3 IRE
					state <= s6_down_95;

				WHEN s6_down_95 =>
					pga1_bb1_pedistal_reg(3 DOWNTO 0) <= "0010"; -- 0,95 IRE
					state <= s7_down0;

				WHEN s7_down0 =>
					pga1_bb1_pedistal_reg(3 DOWNTO 0) <= "0000"; -- 0 IRE
					pga1_bb1_load_pedistal <= '0';
					state <= s0_up_95;

			END CASE;
		END IF;
	END PROCESS;

END PEDISTAL_DATA2DAC;



