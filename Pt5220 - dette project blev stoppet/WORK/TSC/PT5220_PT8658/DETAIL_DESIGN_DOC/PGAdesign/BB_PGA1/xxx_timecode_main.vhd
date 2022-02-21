-- Navn			: BB1_TIMECODE_MAIN
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: This rutine controls the timecode. It recieves data from timecoderegister and
--				: according to the data controls the adr setup for the timecode ram and 
--				: the timecode datastream for the dac.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY xxx_TIMECODE_MAIN IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_tc_type_ready			:	IN	STD_LOGIC;
		pga1_bb1_next_bit				:	IN  STD_LOGIC;
		pga1_bb1_1_timecode				:	IN	STD_LOGIC;
		pga1_bb1_0_timecode				:	IN	STD_LOGIC;

		pga1_bb1_play_tc_adr			:	OUT	INTEGER RANGE 0 TO 127
		);
END xxx_TIMECODE_MAIN;
ARCHITECTURE TIMECODE_MAIN OF xxx_TIMECODE_MAIN IS
	TYPE STATE_TYPE IS (s0_check_tc_type,s1_next_tc_bit);
	SIGNAL state: STATE_TYPE;
	SIGNAL tc_adr_out : INTEGER RANGE 0 TO 127; -- uses 00 = 0's, 127 = 1's
BEGIN
	PROCESS (clk)
	VARIABLE tc_adr_reg : INTEGER RANGE 0 TO 127 := 1; -- uses 00 = 0's, 127 = 1's
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_check_tc_type =>
					IF pga1_bb1_tc_type_ready = '1' THEN
						IF pga1_bb1_1_timecode = '1' THEN
							tc_adr_out <= 127;
							state <= s1_next_tc_bit;
						ELSIF pga1_bb1_0_timecode = '1' THEN
							tc_adr_out <= 0;
							state <= s1_next_tc_bit;
						ELSE
							tc_adr_out <= tc_adr_reg;
							state <= s1_next_tc_bit;
						END IF;
					ELSE
						state <= s0_check_tc_type;
					END IF;
	
				WHEN s1_next_tc_bit =>
					IF pga1_bb1_next_bit = '1' THEN
						tc_adr_reg := tc_adr_reg+1;
						state <= s0_check_tc_type;
					ELSE
						state <= s1_next_tc_bit;
					END IF;

			END CASE;
		END IF;
	END PROCESS;
	pga1_bb1_play_tc_adr <= tc_adr_out; -- out adr to the timecode ram.
END TIMECODE_MAIN;
