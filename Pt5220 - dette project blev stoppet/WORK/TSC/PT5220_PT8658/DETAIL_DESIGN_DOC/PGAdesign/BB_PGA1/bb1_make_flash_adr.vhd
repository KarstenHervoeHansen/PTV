-- Navn			: BB1_MAKE_FLASH_ADR
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: 
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;



ENTITY BB1_MAKE_FLASH_ADR IS
	PORT(
		clk								:	IN	STD_LOGIC;
		Reg_bb1_HphaseCoarse			:	IN	INTEGER RANGE 0 TO 2047;
		Reg_bb1_System					:	IN	INTEGER RANGE 0 TO 1;
		pga1_bb1_tristate_adr			:	IN	BIT;


		pga1_bb1_start_sync				:	IN	BIT;
		pga1_bb1_start_sync				:	IN	BIT;
		pga2_bb3_4_end					:	IN	BIT;

		pga1_bb1_burst_adr				:	IN	STD_LOGIC_VECTOR(10 DOWNTO 0);

		pga1_bb1_flash_adr				:	OUT	INTEGER RANGE 0 TO 2047
		); 
END BB1_MAKE_FLASH_ADR;

ARCHITECTURE BURST_OR_SYNC OF BB1_MAKE_FLASH_ADR IS
	TYPE STATE_TYPE IS (s0_Hfine_to_flash,s1_burst_to_flash);
	SIGNAL state: STATE_TYPE;
	CONSTANT L7_1 : STD_LOGIC_VECTOR(7 DOWNTO 0) := "00000010";

BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS

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
			END CASE;
		END IF;
	END PROCESS;

END BURST_OR_SYNC;



