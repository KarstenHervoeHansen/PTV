-- Navn			: BB1_TC_SEQUENZE
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Receives the serial data from timecoderegister and transform it
--				: to the full timecode datastream with syncbits and a 8bit CRC.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_TC_SEQUENZE IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_play_tc_end			:	IN	BIT; -- end of hole timecode
		pga1_bb1_bit_tc_end				:	IN	BIT; -- end of each timecode bit
		pga1_bb1_G_M					:	IN	STD_LOGIC;
		pga1_bb1_start_play_tc			:	IN	STD_LOGIC;

--		pga1_bb1_Reg_seq				:	OUT INTEGER RANGE 0 TO 2047
		pga1_bb1_Reg_seq				:	OUT INTEGER RANGE 0 TO 15;
		pga1_bb1_restart_bit			:	OUT STD_LOGIC

		);
END BB1_TC_SEQUENZE;

ARCHITECTURE TC_SEQUENZE OF BB1_TC_SEQUENZE IS
	TYPE STATE_TYPE IS (s0_init,s1_test_play_tc,s2_test_msb,s3_13_cnt,s4_15_cnt,s5_reset_offset,s6_14_cnt,s7_dummy);
	SIGNAL state: STATE_TYPE;
--	SIGNAL G_M_out_reg : INTEGER RANGE 0 TO 2047;
	SIGNAL G_M_out_reg : INTEGER RANGE 0 TO 15;
	CONSTANT G_bit : INTEGER := 0;
	CONSTANT M_bit : INTEGER := 0;
	CONSTANT G_restart : INTEGER := 2;
	CONSTANT M_restart : INTEGER := 7;
	CONSTANT G_add : INTEGER := 14; -- The lsb isn't used, but only to fit with 'M'
	CONSTANT M_add : INTEGER := 11;
	CONSTANT G_offset : INTEGER := 58;
	CONSTANT M_offset : INTEGER := 88;

BEGIN
	PROCESS (clk)
	VARIABLE G_M_offset_reg : INTEGER RANGE 0 TO 255;
	VARIABLE add_reg : INTEGER RANGE 0 TO 15;
	
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_init =>
					IF pga1_bb1_start_play_tc = '1' THEN
						state <= s1_test_play_tc;
					ELSIF pga1_bb1_G_M = '1' THEN
						G_M_out_reg <= G_bit;
						add_reg := G_add;
						G_M_offset_reg := G_offset;
						state <= s0_init;
					ELSE
						G_M_out_reg <= M_bit;
						add_reg := M_add;
						G_M_offset_reg := M_offset;
						state <= s0_init;
					END IF;			
						pga1_bb1_restart_bit <= '0';

				WHEN s1_test_play_tc =>
					IF pga1_bb1_play_tc_end = '1' THEN
						state <= s0_init;
					ELSIF pga1_bb1_bit_tc_end = '1' THEN
						state <= s2_test_msb;
					ELSE
						state <= s1_test_play_tc;
					END IF;			
					G_M_out_reg <= G_M_out_reg + 1;

				WHEN s2_test_msb =>
					IF G_M_offset_reg > 127 THEN
						IF pga1_bb1_G_M = '1' THEN
							pga1_bb1_restart_bit <= '1';
							G_M_offset_reg := G_restart;
							state <= s5_reset_offset;
						ELSE
							G_M_offset_reg := M_restart;
							state <= s3_13_cnt;
						END IF;
					ELSE
						state <= s6_14_cnt;
					END IF;
					G_M_out_reg <= G_M_out_reg + 1;

				WHEN s3_13_cnt =>
					G_M_out_reg <= G_M_out_reg + 1;
					state <= s4_15_cnt;

				WHEN s4_15_cnt =>
					G_M_out_reg <= G_M_out_reg + 1;
					pga1_bb1_restart_bit <= '1';
					state <= s5_reset_offset;

				WHEN s5_reset_offset =>
					IF pga1_bb1_G_M = '1' THEN
						G_M_offset_reg := G_M_offset_reg + G_add;
					ELSE	
						G_M_offset_reg := G_M_offset_reg + M_add;
					END IF;
					G_M_out_reg <= 0;
					pga1_bb1_restart_bit <= '0';
					state <= s1_test_play_tc;

				WHEN s6_14_cnt =>
					G_M_out_reg <= G_M_out_reg + 1;
					pga1_bb1_restart_bit <= '1';
					state <= s7_dummy;

				WHEN s7_dummy =>
					IF pga1_bb1_G_M = '1' THEN
						G_M_offset_reg := G_M_offset_reg + G_add;
					ELSE	
						G_M_offset_reg := G_M_offset_reg + M_add;
					END IF;
					G_M_out_reg <= 0;
					pga1_bb1_restart_bit <= '0';
					state <= s1_test_play_tc;

			END CASE;
		END IF;
	END PROCESS;
	pga1_bb1_Reg_seq <= G_M_out_reg;
END TC_SEQUENZE;





