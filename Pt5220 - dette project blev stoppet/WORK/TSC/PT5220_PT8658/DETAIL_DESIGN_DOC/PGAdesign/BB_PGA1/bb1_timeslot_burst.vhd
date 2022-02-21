-- Navn			: BB1_TIMESLOT_BURST.vhd
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Timeslots to the blackburst-loader
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_TIMESLOT_BURST IS
	PORT(
		clk									:	IN	STD_LOGIC;
		pga1_bb1_load_dac_burst				:	IN	BIT;
		pga1_bb2_load_dac_burst				:	IN	BIT;
		pga2_bb3_4_end						:	IN	STD_LOGIC;

		pga1_bb1_load_active				:	OUT	STD_LOGIC;
		pga1_bb2_load_active				:	OUT	STD_LOGIC;
		pga1_bb1_2_end						:	OUT	STD_LOGIC

		); 
END BB1_TIMESLOT_BURST;

ARCHITECTURE TIMESLOT2BURST OF BB1_TIMESLOT_BURST IS
	TYPE STATE_TYPE IS (INIT,s0_Time2Burst1_3,s1_Time2Burst2_4);
	SIGNAL state: STATE_TYPE;

BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN INIT =>
					IF pga2_bb3_4_end = '0' THEN
						state <= INIT;
					ELSIF pga1_bb1_load_dac_burst = '1' THEN
						state <= s0_Time2Burst1_3;
						pga1_bb1_2_end <= '1';
					ELSIF pga1_bb2_load_dac_burst = '1' THEN
						state <= s1_Time2Burst2_4;
						pga1_bb1_2_end <= '1';
					ELSE
						pga1_bb1_2_end <= '1'; -- BB1_2 not ready 
						state <= INIT;
					END IF;
						pga1_bb1_load_active <= '0';
						pga1_bb2_load_active <= '0';


				WHEN s0_Time2Burst1_3 =>
						IF pga1_bb1_load_dac_burst = '1' THEN
							state <= s0_Time2Burst1_3;
							pga1_bb1_2_end <= '1';
							pga1_bb1_load_active <= '1';
						ELSE
						state <= INIT;
					END IF;

				WHEN s1_Time2Burst2_4 =>
						IF pga1_bb2_load_dac_burst = '1' THEN
							state <= s1_Time2Burst2_4;
							pga1_bb1_2_end <= '1';
							pga1_bb2_load_active <= '1';
						ELSE
						state <= INIT;
					END IF;
				WHEN OTHERS =>
						state <= INIT;
				
			END CASE;
		END IF;
	END PROCESS;

END TIMESLOT2BURST;

