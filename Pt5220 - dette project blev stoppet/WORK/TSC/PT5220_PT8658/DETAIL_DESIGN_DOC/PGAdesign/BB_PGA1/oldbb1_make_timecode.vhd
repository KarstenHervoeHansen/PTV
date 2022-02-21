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

		pga1_bb1_tc_data_ok					:	OUT	STD_LOGIC;
		pga1_bb1_tc_load_clk				:	OUT	STD_LOGIC;
		pga1_bb1_next_tc_data				:	OUT	STD_LOGIC;
		pga1_bb1_next_crc_data				:	OUT	STD_LOGIC;
		pga1_bb1_clr_crc					:	OUT	STD_LOGIC;
		pga1_bb1_tcreg90					:	OUT	STD_LOGIC

--		Reg_pga1_bb1_TC_rdy_off				:	OUT	STD_LOGIC;
--		Reg_pga1_bb1_HphaseFine				:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0)

		);
END BB1_MAKE_TIMECODE;

ARCHITECTURE MAKE_TIMECODE OF BB1_MAKE_TIMECODE IS
	TYPE STATE_TYPE IS (s0_init_tc,s1_make_sync_bit,s2_sync0,s3_sync1,
		s4_get_tc_crc,s5_bitreg_inc,s6_next_crc);
	SIGNAL state: STATE_TYPE;
	SIGNAL syncbit : bit;

BEGIN
	PROCESS (clk)
	VARIABLE bytereg : INTEGER RANGE 0 TO 15;
	VARIABLE bitreg  : INTEGER RANGE 0 TO 15;
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_init_tc =>
					IF pga1_bb1_tc_rdy = '1' THEN
						syncbit <= '1';
						pga1_bb1_clr_crc <= '0';
						pga1_bb1_tc_data_ok <= '0';
						state <= s1_make_sync_bit;
					ELSE
						pga1_bb1_clr_crc <= '1';
						bytereg := 15;
						state <= s0_init_tc;
					END IF;
	
				WHEN s1_make_sync_bit =>
					IF syncbit = '1' THEN
						pga1_bb1_tcreg90 <= '1';
						pga1_bb1_tc_load_clk <= '0';
						state <= s2_sync0;
					ELSE
						pga1_bb1_tcreg90 <= '0';
						pga1_bb1_tc_load_clk <= '0';
						state <= s3_sync1;
					END IF;
						bitreg := 15;

				WHEN s2_sync0 =>
						syncbit <= '0';
						pga1_bb1_tc_load_clk <= '1';
						state <= s1_make_sync_bit;

				WHEN s3_sync1 =>
						syncbit <= '1';
						pga1_bb1_tc_load_clk <= '1';
						bytereg := bytereg + 1;
						bitreg := bitreg + 1;
						state <= s4_get_tc_crc;

				WHEN s4_get_tc_crc =>
					IF bitreg = 8 THEN
						syncbit <= '1';
						state <= s1_make_sync_bit;
					ELSIF bytereg > 7 THEN
						state <= s6_next_crc;
					ELSE
						pga1_bb1_next_tc_data <= '1';
						pga1_bb1_tcreg90 <= pga1_bb1_timecode_8x8data;
						pga1_bb1_tc_load_clk <= '0';
						state <= s5_bitreg_inc;
					END IF;

				WHEN s5_bitreg_inc =>
					IF bytereg = 8 THEN
						state <= s6_next_crc;
						pga1_bb1_next_crc_data <= '0';
					ELSE
						state <= s4_get_tc_crc;
					END IF;
					bitreg := bitreg + 1;
					pga1_bb1_tc_load_clk <= '1';

				WHEN s6_next_crc =>
					IF bitreg = 8 THEN
						pga1_bb1_tc_data_ok <= '1';
						state <= s0_init_tc;
					ELSE
						pga1_bb1_tc_load_clk <= '0';
						state <= s5_bitreg_inc;
					END IF;
						pga1_bb1_next_crc_data <= '1';
						pga1_bb1_tcreg90 <= pga1_bb1_crc_data;
				WHEN OTHERS => state <= s0_init_tc;

			END CASE;
		END IF;
	END PROCESS;

END MAKE_TIMECODE;





