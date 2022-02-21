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




ENTITY xxx_TC_SEQUENZE IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_tc_sample				:	IN INTEGER RANGE 0 TO 15;
		pga1_bb1_G_M					:	IN	STD_LOGIC;
		pga1_bb1_start_tc				:	IN	STD_LOGIC;
		pga1_bb1_end_tc					:	IN	STD_LOGIC;

		pga1_bb1_next_bit				:	OUT STD_LOGIC

		);
END xxx_TC_SEQUENZE;

ARCHITECTURE TC_SEQUENZE OF xxx_TC_SEQUENZE IS
	TYPE STATE_TYPE IS (s0_init,s1_tc_seq,s2_1g,s3_9g,s4_1m,s5_11m);
	SIGNAL state: STATE_TYPE;

	CONSTANT G_offset : INTEGER := 1;
	CONSTANT M_offset : INTEGER := 1;
	CONSTANT sampleG_14bit : INTEGER := 14-G_offset;
	CONSTANT sampleG_15bit : INTEGER := 15-G_offset;
	CONSTANT sampleM_15bit : INTEGER := 15-M_offset;
	CONSTANT sampleM_16bit : INTEGER := 16-M_offset;

BEGIN
	PROCESS (clk)
	VARIABLE G_M_offset_reg : INTEGER RANGE 0 TO 15 :=5;
	
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_init =>
					IF (pga1_bb1_G_M = '1' and pga1_bb1_start_tc = '1')THEN
						G_M_offset_reg := 5; -- offset in G
						state <= s1_tc_seq;
					ELSIF (pga1_bb1_G_M = '0' and pga1_bb1_start_tc = '1')THEN
						G_M_offset_reg := 6; -- offset in M
						state <= s1_tc_seq;
					ELSE
						state <= s0_init;
					END IF;
						pga1_bb1_next_bit <= '0';

				WHEN s1_tc_seq =>
					IF pga1_bb1_end_tc = '1' THEN
						state <= s0_init;
					ELSIF pga1_bb1_G_M = '1' THEN
						IF G_M_offset_reg = 9+1 THEN
							state <= s2_1g;
						ELSE
							state <= s3_9g;
						END IF;
					ELSIF G_M_offset_reg = 11+1 THEN
							state <= s4_1m;
						ELSE
							state <= s5_11m;
					END IF;

				WHEN s2_1g =>
					IF pga1_bb1_tc_sample = sampleG_14bit THEN
						pga1_bb1_next_bit <= '1';
						G_M_offset_reg := G_M_offset_reg + 1;
						state <= s1_tc_seq;
					ELSE
						pga1_bb1_next_bit <= '0';
						state <= s2_1g;
					END IF;
					G_M_offset_reg := 1;

				WHEN s3_9g =>
					IF pga1_bb1_tc_sample = sampleG_14bit THEN
						pga1_bb1_next_bit <= '1';
						G_M_offset_reg := G_M_offset_reg + 1;
						state <= s1_tc_seq;
					ELSE
						pga1_bb1_next_bit <= '0';
						state <= s3_9g;
					END IF;

				WHEN s4_1m =>
					IF pga1_bb1_tc_sample = sampleG_14bit THEN
						pga1_bb1_next_bit <= '1';
						G_M_offset_reg := G_M_offset_reg + 1;
						state <= s1_tc_seq;
					ELSE
						pga1_bb1_next_bit <= '0';
						state <= s4_1m;
					END IF;
					G_M_offset_reg := 1;

				WHEN s5_11m =>
					IF pga1_bb1_tc_sample = sampleG_14bit THEN
						pga1_bb1_next_bit <= '1';
						G_M_offset_reg := G_M_offset_reg + 1;
						state <= s1_tc_seq;
					ELSE
						pga1_bb1_next_bit <= '0';
						state <= s5_11m;
					END IF;

			END CASE;
		END IF;
	END PROCESS;
END TC_SEQUENZE;





