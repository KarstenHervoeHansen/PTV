-- Navn			: BB1_MAKE_TIMECODE
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




ENTITY BB1_MAKE_TIMECODE IS
	PORT(
		clk									:	IN	STD_LOGIC;
		pga1_bb1_crc_data					:	IN	STD_LOGIC;
		pga1_bb1_timecode_8x8data			:	IN	STD_LOGIC;
		pga1_bb1_tc_rdy						:	IN	STD_LOGIC;
--		pga1_bb1_tc_on_off					:	IN	STD_LOGIC;

		pga1_bb1_tc_data_ok					:	OUT	STD_LOGIC;
		pga1_bb1_tc_crc_clk					:	OUT	STD_LOGIC;
		pga1_bb1_next_tc_data				:	OUT	STD_LOGIC;
		pga1_bb1_ser_reload					:	OUT STD_LOGIC;
		pga1_bb1_clr_crc					:	OUT	STD_LOGIC;
		pga1_bb1_tcreg90					:	OUT	STD_LOGIC

		);
END BB1_MAKE_TIMECODE;

ARCHITECTURE MAKE_TIMECODE OF BB1_MAKE_TIMECODE IS
	TYPE STATE_TYPE IS (s0_init_tc,s1_tc_count,s2_tc_reload);
	SIGNAL state: STATE_TYPE;
	SIGNAL syncbit : bit;

BEGIN
	PROCESS (clk)
	VARIABLE tc_data_count : INTEGER RANGE 0 TO 90;
--	VARIABLE bytereg : INTEGER RANGE 0 TO 15;
--	VARIABLE bitreg  : INTEGER RANGE 0 TO 15;
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_init_tc =>
					IF pga1_bb1_tc_rdy = '0' THEN
						pga1_bb1_clr_crc <= '1';
						pga1_bb1_next_tc_data <= '0';
						pga1_bb1_tc_data_ok <= '0';
						pga1_bb1_tc_crc_clk <= '0';
						tc_data_count := 0;
						pga1_bb1_ser_reload <= '1';
						state <= s0_init_tc;

					ELSIF tc_data_count = 90 THEN
						pga1_bb1_tc_data_ok <= '1';
						state <= s2_tc_reload;

					ELSIF tc_data_count > 81 THEN
						pga1_bb1_next_tc_data <= '1';
						pga1_bb1_tcreg90 <= pga1_bb1_crc_data;
						pga1_bb1_clr_crc <= '0';
						pga1_bb1_tc_crc_clk <= '1';
						state <= s1_tc_count;
					ELSE
						pga1_bb1_next_tc_data <= '1';
						pga1_bb1_tcreg90 <= pga1_bb1_timecode_8x8data;
						pga1_bb1_tc_crc_clk <= '1';
						pga1_bb1_clr_crc <= '0';
						state <= s1_tc_count;
					END IF;
	
				WHEN s1_tc_count =>
						tc_data_count := tc_data_count +1;
						pga1_bb1_next_tc_data <= '0';
						pga1_bb1_tc_crc_clk <= '0';
						state <= s0_init_tc;

				WHEN s2_tc_reload =>
					IF pga1_bb1_tc_rdy = '1' THEN
						pga1_bb1_ser_reload <= '0';
						state <= s2_tc_reload;
					ELSE
						state <= s0_init_tc;
					END IF;

				WHEN OTHERS =>
						state <= s0_init_tc;

			END CASE;
		END IF;
	END PROCESS;

END MAKE_TIMECODE;





