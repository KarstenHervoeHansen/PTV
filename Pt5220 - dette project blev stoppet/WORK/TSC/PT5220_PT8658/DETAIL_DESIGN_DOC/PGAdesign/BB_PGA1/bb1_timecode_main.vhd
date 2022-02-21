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




ENTITY BB1_TIMECODE_MAIN IS
	PORT(
		clk									:	IN	STD_LOGIC;
		pga1_bb1_tc_data_ok					:	IN	STD_LOGIC;
		pga1_bb1_start_timecode				:	IN	STD_LOGIC;
		pga1_bb1_tc_out_90bit				:	IN	STD_LOGIC;
		pga1_bb1_bit_tc_end					:	IN	STD_LOGIC;
		pga1_bb1_play_tc_end				:	IN	STD_LOGIC;

		pga1_bb1_timecode_clk				:	OUT	STD_LOGIC; -- clk to 90bit tc reg.
		pga1_bb1_play_tc_adr					:	OUT	INTEGER RANGE 0 TO 127;
--STD_LOGIC_VECTOR(6 DOWNTO 0);
		pga1_bb1_tc_down_up					:	OUT	STD_LOGIC;	--'0'= from 1 to 0
															--'1'= from 0 to 1
		pga1_bb1_start_play_tc				:	OUT	STD_LOGIC
		);
END BB1_TIMECODE_MAIN;

ARCHITECTURE TIMECODE_MAIN OF BB1_TIMECODE_MAIN IS
	TYPE STATE_TYPE IS (s0_check_tc_data,s1_check_tc_start,s2_tc_00,s3_tc_01,
		s4_tc_clk_10,s5_tc_clk_11,s6_tc_data_10,s7_tc_data_11);
	SIGNAL state: STATE_TYPE;
	SIGNAL tc_adr_out : INTEGER RANGE 0 TO 127; -- uses 91 = 0's, 92 = 1's


BEGIN
	PROCESS (clk)
--	VARIABLE tc_adr_out : INTEGER RANGE 0 TO 127; -- uses 1 to 90 for timecode.
	VARIABLE tc_adr_reg : INTEGER RANGE 0 TO 127; -- uses 91 = 0's, 92 = 1's
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_check_tc_data =>
					IF pga1_bb1_tc_data_ok = '1' THEN
						state <= s1_check_tc_start;
					ELSE
						tc_adr_reg := 0;
						pga1_bb1_start_play_tc <= '0';
						state <= s0_check_tc_data;
					END IF;
	
				WHEN s1_check_tc_start =>
					IF pga1_bb1_start_timecode = '1' THEN
						IF pga1_bb1_tc_out_90bit = '0' THEN
							state <= s2_tc_00;
						ELSE
							state <= s3_tc_01;
						END IF;
					ELSE
						state <= s1_check_tc_start;
					END IF;

				WHEN s2_tc_00 =>
					tc_adr_out <= 91; -- play '0' (black)
					pga1_bb1_start_play_tc <= '1';
					pga1_bb1_timecode_clk <= '1';
					state <= s4_tc_clk_10;

				WHEN s3_tc_01 =>
					tc_adr_reg := tc_adr_reg + 1; -- next tc data (black)
					pga1_bb1_start_play_tc <= '1';
					pga1_bb1_timecode_clk <= '1';
					state <= s5_tc_clk_11;

				WHEN s4_tc_clk_10 =>
					pga1_bb1_timecode_clk <= '0';
					state <= s6_tc_data_10;

				WHEN s5_tc_clk_11 =>
					pga1_bb1_timecode_clk <= '0';
					state <= s7_tc_data_11;

				WHEN s6_tc_data_10 =>
					IF pga1_bb1_tc_out_90bit = '0' THEN
						IF pga1_bb1_play_tc_end = '1' THEN
							state <= s0_check_tc_data;
							ELSIF pga1_bb1_bit_tc_end = '1' THEN
								pga1_bb1_start_play_tc <= '1';
								tc_adr_out <= 91;
								pga1_bb1_timecode_clk <= '1';
								tc_adr_reg := tc_adr_reg + 1;
								state <= s4_tc_clk_10;
							ELSE
								state <= s6_tc_data_10;
							END IF;
--						END IF;
--					END IF;
--					IF pga1_bb1_tc_out_90bit = '1' THEN
					ELSIF pga1_bb1_play_tc_end = '1' THEN
						state <= s0_check_tc_data;
							ELSIF pga1_bb1_bit_tc_end = '1' THEN
								pga1_bb1_start_play_tc <= '1'; -- go for it
								tc_adr_out <= tc_adr_reg + 1; -- out actual adr+1
								pga1_bb1_timecode_clk <= '1'; --get next tc data from 90bit reg
								tc_adr_reg := tc_adr_reg + 1; --actual adr+1 
								pga1_bb1_tc_down_up <= '1'; -- up!
								state <= s4_tc_clk_10;
							ELSE
								state <= s7_tc_data_11;
							END IF;
--						END IF;
--					END IF;

				WHEN s7_tc_data_11 =>
					IF pga1_bb1_tc_out_90bit = '0' THEN
						IF pga1_bb1_play_tc_end = '1' THEN
							state <= s0_check_tc_data;
							ELSIF pga1_bb1_bit_tc_end = '1' THEN
								pga1_bb1_start_play_tc <= '1';
								tc_adr_out <= 92;
								pga1_bb1_timecode_clk <= '1';
								tc_adr_reg := tc_adr_reg + 1;
								state <= s5_tc_clk_11;
							ELSE
								state <= s7_tc_data_11;
--							END IF;
						END IF;
					ELSIF pga1_bb1_play_tc_end = '1' THEN
						state <= s0_check_tc_data;
							ELSIF pga1_bb1_bit_tc_end = '1' THEN
								pga1_bb1_start_play_tc <= '1'; -- go for it
								tc_adr_out <= tc_adr_reg + 1; -- out actual adr+1
								pga1_bb1_timecode_clk <= '1'; --get next tc data from 90bit reg
								tc_adr_reg := tc_adr_reg + 1; --actual adr+1 
								pga1_bb1_tc_down_up <= '1'; -- up!
								state <= s5_tc_clk_11;
							ELSE
								state <= s6_tc_data_10;
--							END IF;
--						END IF;
					END IF;
			END CASE;
		END IF;
	END PROCESS;
	pga1_bb1_play_tc_adr <= tc_adr_out; -- out adr to the timecode ram.
END TIMECODE_MAIN;
