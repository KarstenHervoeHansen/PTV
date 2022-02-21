-- Navn			: BB1_TC_SIG_TYPE
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




ENTITY xxx_TC_SIG_TYPE IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_start_tc				:	IN	STD_LOGIC;
		pga1_bb1_end_tc					:	IN	STD_LOGIC;
		pga1_bb1_next_bit				:	IN  STD_LOGIC;
		pga1_bb1_tc_90bit				:	IN  STD_LOGIC;

		pga1_bb1_1_timecode				:	OUT	STD_LOGIC;
		pga1_bb1_0_timecode				:	OUT	STD_LOGIC;
		pga1_bb1_u_d_timecode			:	OUT	STD_LOGIC;
--		pga1_bb1_play_tc_adr			:	OUT INTEGER RANGE 0 TO 15;
		pga1_bb1_tc_type_ready			:	OUT	STD_LOGIC;
		pga1_bb1_tc_adr_tri				:	OUT	STD_LOGIC
		);
END xxx_TC_SIG_TYPE;
ARCHITECTURE TIMECODE_MAIN OF xxx_TC_SIG_TYPE IS
	TYPE STATE_TYPE IS (s0_init_tc_type,s1_tc_0_bit,s2_tc_1_bit);
	SIGNAL state: STATE_TYPE;
--	SIGNAL tc_adr_out : INTEGER RANGE 0 TO 127 := 1; -- uses 00 = 0's, 127 = 1's
BEGIN
	PROCESS (clk)
--	VARIABLE tc_adr_reg : INTEGER RANGE 0 TO 127 := 1; -- uses 00 = 0's, 127 = 1's
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_init_tc_type =>
					IF pga1_bb1_start_tc = '1' THEN
						pga1_bb1_tc_adr_tri <= '1';
						IF pga1_bb1_tc_90bit = '0' THEN
--							tc_adr_out <= 0;
							state <= s1_tc_0_bit;
						ELSE
							state <= s2_tc_1_bit;
						END IF;
					ELSE
						pga1_bb1_tc_adr_tri <= '0';
						state <= s0_init_tc_type;
					END IF;
					pga1_bb1_1_timecode	<=	'0';
					pga1_bb1_0_timecode	<=	'0';
					pga1_bb1_u_d_timecode	<=	'1';
					pga1_bb1_tc_type_ready	<=	'0';
	
				WHEN s1_tc_0_bit =>
					IF pga1_bb1_end_tc = '1' THEN
						state <= s0_init_tc_type;
					ELSIF pga1_bb1_next_bit = '1' THEN
						state <= s1_tc_0_bit;
					ELSIF pga1_bb1_tc_90bit = '0' THEN
						pga1_bb1_0_timecode <= '1';
						state <= s1_tc_0_bit;
					ELSE
						pga1_bb1_u_d_timecode <= '1';
						pga1_bb1_0_timecode <= '0';
						state <= s2_tc_1_bit;
					END IF;
--						tc_adr_out <= tc_adr_out+1;

				WHEN s2_tc_1_bit =>
					IF pga1_bb1_end_tc = '1' THEN
						state <= s0_init_tc_type;
					ELSIF pga1_bb1_next_bit = '1' THEN
						state <= s2_tc_1_bit;
					ELSIF pga1_bb1_tc_90bit = '0' THEN
						pga1_bb1_u_d_timecode <= '0';
						pga1_bb1_1_timecode <= '0';
						state <= s1_tc_0_bit;
					ELSE
						pga1_bb1_1_timecode <= '1';
						state <= s2_tc_1_bit;
					END IF;
--						tc_adr_out <= tc_adr_out+1;

				WHEN OTHERS =>
						state <= s0_init_tc_type;

			END CASE;
		END IF;
	END PROCESS;
--	pga1_bb1_play_tc_adr <= tc_adr_out; -- out adr to the timecode ram.
END TIMECODE_MAIN;
