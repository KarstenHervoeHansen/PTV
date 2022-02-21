-- Navn			: BB1_PLAY_TIMECODE
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Setup timecode-adr and Writes data from the timecode-ram to the dac
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_PLAY_TIMECODE IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_start_play_tc			:	IN	STD_LOGIC;
		pga1_bb1_G_M					:	IN	STD_LOGIC;
		pga1_bb1_Reg_seq				:	IN	INTEGER RANGE 0 TO 15; --holds actual tc-bit
		pga1_Reg_bb1_HphaseCoarse		:	IN	INTEGER RANGE 0 TO 2047; --holds the Hphase
		pga1_bb1_restart_bit			:	IN	STD_LOGIC;

		pga1_bb1_play_tc_end			:	OUT	BIT; -- end of hole timecode
		pga1_bb1_bit_tc_end				:	OUT	BIT; -- end of each timecode bit
		pga1_bb1_load_timecode			:	OUT	BIT; -- to dac rutine
		pga1_bb1_timecode_ram_adr		:	OUT	INTEGER RANGE 0 TO 15
		); 
END BB1_PLAY_TIMECODE;

ARCHITECTURE TIMECODE_DATA2DAC OF BB1_PLAY_TIMECODE IS
	TYPE STATE_TYPE IS (s0_init_timecode_adr,s1_get_timecode_data);
	SIGNAL state: STATE_TYPE;
	SIGNAL adr_count : INTEGER RANGE 0 TO 15;
	CONSTANT G_offset : INTEGER := 303;
	CONSTANT M_offset : INTEGER := 270;
	CONSTANT G_tc_end : INTEGER := 1341; -- end of timecode in 'G'
	CONSTANT M_tc_end : INTEGER := 1358; -- end of timecode in 'M'


BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS

				WHEN s0_init_timecode_adr =>

					IF pga1_bb1_start_play_tc = '1' THEN
						pga1_bb1_load_timecode <= '1';
						pga1_bb1_play_tc_end <= '0'; --play tc not finished
						state <= s1_get_timecode_data;
					ELSE
						pga1_bb1_load_timecode <= '0';
						pga1_bb1_play_tc_end <= '1'; --play tc not finished
						state <= s0_init_timecode_adr;
					END IF;
						adr_count <= 0;
						pga1_bb1_bit_tc_end <= '0'; --tc signal not finished
		
				WHEN s1_get_timecode_data =>
					IF ((pga1_bb1_G_M = '1') AND (pga1_Reg_bb1_HphaseCoarse =  G_tc_end-1))
						OR ((pga1_bb1_G_M = '0') AND (pga1_Reg_bb1_HphaseCoarse =  M_tc_end-1)) THEN
						pga1_bb1_play_tc_end <= '1';
						pga1_bb1_load_timecode <= '0'; -- stop dac load
						state <= s0_init_timecode_adr;
					ELSIF pga1_bb1_Reg_seq = 10 THEN
						pga1_bb1_bit_tc_end <= '1';
						adr_count <= adr_count + 1;
						state <= s1_get_timecode_data;
					ELSIF pga1_bb1_restart_bit = '1' THEN
						pga1_bb1_bit_tc_end <= '0';
						adr_count <= 0;
						state <= s1_get_timecode_data;
					ELSE
						adr_count <= adr_count + 1;
						state <= s1_get_timecode_data;
					END IF;
--							pga1_bb1_bit_tc_end <= '1';
--							state <= s1_get_timecode_data;
--						ELSIF pga1_bb1_Reg_seq = 14 THEN
--							pga1_bb1_bit_tc_end <= '0';
--							state <= s0_init_timecode_adr;
--						ELSIF pga1_Reg_bb1_HphaseCoarse = G_tc_end+G_offset THEN
--							pga1_bb1_play_tc_end <= '1';
--							state <= s0_init_timecode_adr;
--						ELSE
--							state <= s1_get_timecode_data;
--						END IF;

--					ELSIF (pga1_bb1_Reg_seq = 14) OR (pga1_Reg_bb1_HphaseCoarse = 14+G_offset) THEN
--							pga1_bb1_bit_tc_end <= '1';
--							state <= s1_get_timecode_data;
--						ELSIF pga1_bb1_Reg_seq = 15 THEN
--							pga1_bb1_bit_tc_end <= '0';
--							state <= s0_init_timecode_adr;
--						ELSIF pga1_Reg_bb1_HphaseCoarse = M_tc_end+M_offset THEN
--							pga1_bb1_play_tc_end <= '1';
--							state <= s0_init_timecode_adr;
--						ELSE
--							state <= s1_get_timecode_data;
--					END IF;
--						pga1_bb1_load_timecode <= '1';

			END CASE;
		END IF;
	END PROCESS;
	pga1_bb1_timecode_ram_adr <= adr_count;

END TIMECODE_DATA2DAC;



