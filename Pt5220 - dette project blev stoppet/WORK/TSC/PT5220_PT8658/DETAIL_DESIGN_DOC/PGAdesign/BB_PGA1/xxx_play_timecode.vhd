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




ENTITY xxx_PLAY_TIMECODE IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_start_play_tc			:	IN	STD_LOGIC;
		pga1_bb1_next_bit				:	IN  STD_LOGIC;
		pga1_bb1_end_tc					:	IN	BIT; -- end of hole timecode

		pga1_bb1_timecode_ram_adr		:	OUT	INTEGER RANGE 0 TO 15
		); 
END xxx_PLAY_TIMECODE;

ARCHITECTURE TIMECODE_DATA2DAC OF xxx_PLAY_TIMECODE IS
	TYPE STATE_TYPE IS (s0_init_tc_adr,s1_play_tc_adr);
	SIGNAL state: STATE_TYPE;
	SIGNAL adr_count : INTEGER RANGE 0 TO 15;


BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS

				WHEN s0_init_tc_adr =>

					IF pga1_bb1_start_play_tc = '1' THEN
						state <= s1_play_tc_adr;
					ELSE
						state <= s0_init_tc_adr;
					END IF;
						adr_count <= 0;
		
				WHEN s1_play_tc_adr =>
					IF pga1_bb1_end_tc = '1' THEN
						state <= s0_init_tc_adr;
					ELSIF pga1_bb1_next_bit = '1' THEN
						adr_count <= 0;
						state <= s1_play_tc_adr;
					ELSE
						adr_count <= adr_count + 1;
						state <= s1_play_tc_adr;
					END IF;

			END CASE;
		END IF;
	END PROCESS;
	pga1_bb1_timecode_ram_adr <= adr_count;

END TIMECODE_DATA2DAC;



